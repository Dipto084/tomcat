package edu.arizona.tomcat.Mission;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.microsoft.Malmo.MalmoMod;
import com.microsoft.Malmo.Schemas.PosAndDirection;
import edu.arizona.tomcat.Emotion.EmotionHandler;
import edu.arizona.tomcat.Messaging.TomcatClientServerHandler;
import edu.arizona.tomcat.Messaging.TomcatMessageData;
import edu.arizona.tomcat.Messaging.TomcatMessaging;
import edu.arizona.tomcat.Messaging.TomcatMessaging.TomcatMessage;
import edu.arizona.tomcat.Messaging.TomcatMessaging.TomcatMessageType;
import edu.arizona.tomcat.Mission.gui.FeedbackListener;
import edu.arizona.tomcat.Mission.gui.SelfReportContent;
import edu.arizona.tomcat.Utils.Converter;
import edu.arizona.tomcat.Utils.MinecraftServerHelper;
import edu.arizona.tomcat.Utils.MinecraftVanillaAIHandler;
import edu.arizona.tomcat.World.DrawingHandler;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Set;
import net.minecraft.entity.Entity;
import net.minecraft.entity.EntityLiving;
import net.minecraft.entity.monster.EntitySkeleton;
import net.minecraft.entity.monster.EntityZombie;
import net.minecraft.entity.passive.EntityVillager;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.entity.player.EntityPlayerMP;
import net.minecraft.util.EntitySelectors;
import net.minecraft.world.World;
import net.minecraftforge.common.MinecraftForge;
import net.minecraftforge.event.CommandEvent;
import net.minecraftforge.event.entity.living.LivingDeathEvent;
import net.minecraftforge.fml.common.eventhandler.EventPriority;
import net.minecraftforge.fml.common.eventhandler.SubscribeEvent;

public abstract class Mission implements FeedbackListener, PhaseListener {

  public static enum ID { TUTORIAL, ZOMBIE, USAR_SINGLE_PLAYER }
  ;

  private static final long ENTITY_DELETION_DELAY = 1;
  private static final String SELF_REPORT_FOLDER = "saves/self_reports";
  private HashMap<String, MissionSelfReport> selfReportPerPlayer;

  protected ID id;
  protected static final int REMAINING_SECONDS_ALERT = 30;
  private boolean canShowSelfReport;
  protected boolean missionPaused;
  protected boolean worldFrozen;
  protected boolean missionSentToClients;
  protected int numberOfPhasesCompleted;
  protected long timeLimitInSeconds;
  protected long selfReportPromptTimeInSeconds;
  protected long initialWorldTime;
  protected long worldTimeOnPause;
  protected long pausedTime;
  protected DrawingHandler drawingHandler;
  protected EmotionHandler.Emotion currentEmotion;
  protected MissionPhase currentPhase;
  protected ArrayList<MissionPhase> phases;
  protected ArrayList<MissionListener> listeners;
  protected HashMap<Entity, Long> entitiesToRemove;

  /**
   * Abstract constructor for initialization of the drawing handler
   */
  protected Mission() {
    this.drawingHandler = DrawingHandler.getInstance();
    this.listeners = new ArrayList<MissionListener>();
    this.selfReportPerPlayer = new HashMap<String, MissionSelfReport>();
    this.canShowSelfReport = false;
    this.missionPaused = false;
    this.worldFrozen = false;
    this.missionSentToClients = false;
    this.entitiesToRemove = new HashMap<Entity, Long>();
    MinecraftForge.EVENT_BUS.register(this);
  }

  @SubscribeEvent
  public void PlayerDeath(LivingDeathEvent event) {
    if (!event.getEntity().world.isRemote && event.getEntity() instanceof
                                                 EntityPlayer) {
      event.setCanceled(true);
      this.onPlayerDeath((EntityPlayer)event.getEntity());
    }
  }

  @SubscribeEvent(priority = EventPriority.HIGHEST)
  public void CommandEvents(CommandEvent evt) {
    System.out.println("================>COMMAND");
    System.out.println(evt.getSender().getName());
    System.out.println(evt.getCommand().getName());
    if (evt.getSender() instanceof EntityPlayer) {
      System.out.println("============>Player sent event");
    }
  }

  /**
   * Method called after if the player dies
   */
  protected abstract void onPlayerDeath(EntityPlayer player);

  /**
   * Adds listener to be notified upon relevant mission events
   * @param listener - Mission listener object
   */
  public void addListener(MissionListener listener) {
    this.listeners.add(listener);
  }

  /**
   * Create phases of the mission. Implemented in the subclasses.
   */
  protected abstract void createPhases();

  /**
   * Initialize data for the mission.
   * @param world - Mission world
   */
  public void init(World world) {
    this.numberOfPhasesCompleted = 0;
    this.phases = new ArrayList<MissionPhase>();
    this.initialWorldTime = world.getTotalWorldTime();
    this.createPhases();
    if (!this.phases.isEmpty()) {
      this.currentPhase = this.phases.get(0);
    }
    this.setServerInfo();
  };

  /**
   * Initialize client and server mission objects in the active MalmoMod
   * instance
   */
  public void setServerInfo() {
    MalmoMod.instance.getServer().setTomcatServerMission(this);
    QuitProducer quitProducerHandler = new QuitProducer();
    this.addListener(quitProducerHandler);
    MalmoMod.instance.getServer().addQuitProducer(quitProducerHandler);
  }

  /**
   * Updates the mission from time to time. This method is called at every tick.
   * @param world - Mission world
   */
  public void update(World world) {
    // These two initializations need to be here because the clients are not yet
    // ready in the init method in a multiplayer setting
    this.initMissionOnClients();
    this.initSelfReports(world);
    if (this.missionPaused) {
      this.freezeWorld(world);
    }
    else {
      this.unfreezeWorld(world);
      if (this.timeLimitInSeconds > 0) {
        int remainingSeconds = this.getRemainingSeconds(world);

        if (remainingSeconds >= 0 || this.timeLimitInSeconds == -1) {
          this.askClientsToUpdateCountdown(remainingSeconds);
          this.updateOnRunningState(world);
        }
        else {
          this.onTimeOut();
        }
      }
      else {
        this.updateOnRunningState(world);
      }
    }
  }

  /**
   * Freeze entities to create an effect of pausing the world in a multiplayer
   * game
   */
  private void freezeWorld(World world) {
    if (!this.worldFrozen) {
      for (EntityLiving entity :
           world.getEntities(EntityLiving.class, EntitySelectors.IS_ALIVE)) {
        if (entity instanceof EntityZombie) {
          MinecraftVanillaAIHandler.freezeZombie((EntityZombie)entity);
        }
        else if (entity instanceof EntitySkeleton) {
          MinecraftVanillaAIHandler.freezeSkeleton((EntitySkeleton)entity);
        }
        else if (entity instanceof EntityVillager) {
          MinecraftVanillaAIHandler.freezeVillager((EntityVillager)entity);
        }
      }
      this.worldFrozen = true;
    }
  }

  /**
   * Freeze entities to create an effect of pausing the world in a multiplayer
   * game
   */
  private void unfreezeWorld(World world) {
    if (this.worldFrozen) {
      for (EntityLiving entity :
           world.getEntities(EntityLiving.class, EntitySelectors.IS_ALIVE)) {
        if (entity instanceof EntityZombie) {
          MinecraftVanillaAIHandler.unfreezeZombie((EntityZombie)entity);
        }
        else if (entity instanceof EntitySkeleton) {
          MinecraftVanillaAIHandler.unfreezeSkeleton((EntitySkeleton)entity);
        }
        else if (entity instanceof EntityVillager) {
          MinecraftVanillaAIHandler.unfreezeVillager((EntityVillager)entity);
        }
      }
      this.worldFrozen = false;
    }
  }

  /**
   * Asks the clients to update the countdown
   * @param remainingSeconds
   */
  private void askClientsToUpdateCountdown(int remainingSeconds) {
    TomcatMessageData messageData = new TomcatMessageData();
    messageData.setRemainingSeconds(remainingSeconds);
    messageData.setRemainingSecondsAlert(REMAINING_SECONDS_ALERT);
    TomcatMessaging.TomcatMessage message =
        new TomcatMessage(TomcatMessageType.UPDATE_COUNTDOWN, messageData);
    TomcatClientServerHandler.sendMessageToAllClients(message, false);
  }

  /**
   * Send message to the clients so they can create their ClientMission object
   */
  private void initMissionOnClients() {
    if (!this.missionSentToClients) {
      for (EntityPlayerMP player : MinecraftServerHelper.getPlayers()) {
        TomcatMessageData messageData = new TomcatMessageData();
        messageData.setMissionID(this.id);
        TomcatMessaging.TomcatMessage message =
            new TomcatMessage(TomcatMessageType.INIT_MISSION, messageData);
        TomcatClientServerHandler.sendMessageToClient(player, message, false);
      }
      this.missionSentToClients = true;
    }
  }

  /**
   * Updates the mission if it's not timed out
   */
  private void updateOnRunningState(World world) {
    this.removeEntities(world);
    this.updateCurrentPhase(world);
    this.updateScene(world);
    this.showSelfReportScreen(world);
  }

  private void removeEntities(World world) {
    Set<Entity> entities = this.entitiesToRemove.keySet();
    for (Entity entity : entities) {
      int remainingTime = Converter.getRemainingTimeInSeconds(
          world, this.entitiesToRemove.get(entity), ENTITY_DELETION_DELAY);
      if (remainingTime <= 0) {
        world.removeEntity(entity);
        this.entitiesToRemove.remove(entity);
      }
    }
  }

  /**
   * Create a self-report for each player in the game
   * @param world - Minecraft world
   */
  private void initSelfReports(World world) {
    if (this.hasSelfReport()) {
      if (this.selfReportPerPlayer.isEmpty()) {
        Date missionStartTime = new Date();
        for (EntityPlayer player : world.playerEntities) {
          MissionSelfReport selfReport = new MissionSelfReport(
              this.id.ordinal(), missionStartTime, player.getName());
          this.selfReportPerPlayer.put(player.getName(), selfReport);
        }
      }
    }
  }

  /**
   * Retrieves the remaining seconds until the end of the mission
   * @return
   */
  protected int getRemainingSeconds(World world) {
    return Converter.getRemainingTimeInSeconds(world,
                                               this.initialWorldTime +
                                                   this.pausedTime,
                                               this.timeLimitInSeconds);
  }

  /**
   * Update the current phase of the mission if there's any
   * @param world
   */
  protected void updateCurrentPhase(World world) {
    if (this.currentPhase != null) {
      this.currentPhase.update(world);
    }
  }

  /**
   * Updates the mission scene programmatically. Must be implemented at the
   * subclass level.
   * @param world
   */
  protected abstract void updateScene(World world);

  private void showSelfReportScreen(World world) {
    if (this.hasSelfReport()) {
      long elapsedTime = Converter.getElapsedTimeInSeconds(
          world, this.initialWorldTime + this.pausedTime);
      if (elapsedTime % this.selfReportPromptTimeInSeconds ==
          this.selfReportPromptTimeInSeconds / 2) {
        // It's been passed enough time since last self-report screen was
        // prompted. Allow the game to prompt another self-report screen when it
        // comes the time to do so
        this.canShowSelfReport = true;
      }

      if (elapsedTime % this.selfReportPromptTimeInSeconds == 0 &&
          this.canShowSelfReport) {
        for (EntityPlayerMP player : MinecraftServerHelper.getPlayers()) {
          SelfReportContent content = this.getSelfReportContent(player, world);
          TomcatMessage message =
              new TomcatMessage(TomcatMessageType.SHOW_SELF_REPORT,
                                new TomcatMessageData(content));
          TomcatClientServerHandler.sendMessageToClient(player, message, true);
        }
        this.canShowSelfReport = false;
      }
    }
  }

  /**
   * Checks whether the mission has self-report or not
   * @return
   */
  protected abstract boolean hasSelfReport();

  /**
   * Gets the self-report content (questions and choices) for the mission
   * @return
   */
  protected abstract SelfReportContent
  getSelfReportContent(EntityPlayerMP player, World world);

  @Override
  public void emotionProvided(EmotionHandler.Emotion emotion) {
    this.currentEmotion = emotion;
  }

  /**
   * Adds a phase to the mission
   * @param phase - Mission phase
   */
  public void addPhase(MissionPhase phase) {
    phase.addListener(this);
    this.phases.add(phase);
  }

  @Override
  public void phaseCompleted() {
    this.beforePhaseTrasition();
    this.numberOfPhasesCompleted++;
    this.currentPhase = null;
    if (this.numberOfPhasesCompleted < this.phases.size()) {
      this.currentPhase = this.phases.get(this.numberOfPhasesCompleted);
    }
    else {
      this.afterLastPhaseCompletion();
    }
  }

  /**
   * Method called after the current phase is completed and before the next
   * phase is selected. It can be implemented in a concrete mission class to
   * provide specific logic.
   */
  protected abstract void beforePhaseTrasition();

  /**
   * Method called after the last phase of the mission is completed.
   */
  protected abstract void afterLastPhaseCompletion();

  /**
   * Method called after the the total time for the mission has passed.
   */
  protected abstract void onTimeOut();

  /**
   * Handle message from the client side
   */
  public void handleMessageFromClient(EntityPlayerMP player,
                                      TomcatMessage message) {
    switch (message.getMessageType()) {
    case OPEN_SCREEN_DISMISSED:
      if (TomcatClientServerHandler.haveAllClientsReplied()) {
        this.unpauseMission();
        this.currentPhase.handleDismissalOfOpenScreen();
      }
      else {
        this.showWaitingForOthersScreen(player);
      }
      break;

    case SELF_REPORT_ANSWERED:
      SelfReportContent content = message.getMessageData().getSelfReport();
      this.selfReportPerPlayer.get(player.getName()).addContent(content);
      if (TomcatClientServerHandler.haveAllClientsReplied()) {
        this.unpauseMission();
      }
      else {
        this.showWaitingForOthersScreen(player);
      }
      break;

    case DISPLAY_INSTRUCTIONS:
      // TODO: fix this later so a single player can open the instructions
      // without affect the others
      this.currentPhase.showInstructions();

    case CONNECTION_ERROR:
      this.onTimeOut();

    default:
      break;
    }
  }

  /**
   * Does the configurations necessary to resume the mission in the server
   */
  private void unpauseMission() {
    this.dismissWaitingForOthersScreen();
    this.missionPaused = false;
    this.pausedTime += (MinecraftServerHelper.getServer()
                            .getEntityWorld()
                            .getTotalWorldTime() -
                        this.worldTimeOnPause);
  }

  /**
   * Stores the world time when paused
   */
  public void pauseMission() {
    this.missionPaused = true;
    this.worldTimeOnPause =
        MinecraftServerHelper.getServer().getEntityWorld().getTotalWorldTime();
  }

  /**
   * Dismisses the message screen that informs the player he should wait for the
   * others
   */
  private void dismissWaitingForOthersScreen() {
    TomcatMessaging.TomcatMessage message =
        new TomcatMessage(TomcatMessageType.DISMISS_OPEN_SCREEN);
    TomcatClientServerHandler.sendMessageToAllClients(message, false);
  }

  /**
   * When a player tries to dismiss a screen but others are still with their
   * screens prompted, show a message a screen saying the player he smust wait
   * for the others to continue the mission.
   * @param player
   */
  private void showWaitingForOthersScreen(EntityPlayerMP player) {
    TomcatMessageData messageData = new TomcatMessageData();
    messageData.setMissionPhaseMessage(
        "Waiting for other players' confirmation.");
    TomcatMessaging.TomcatMessage message =
        new TomcatMessage(TomcatMessageType.SHOW_MESSAGE_SCREEN, messageData);
    TomcatClientServerHandler.sendMessageToClient(player, message, false);
  }

  public abstract PosAndDirection
  getPlayersInitialPositionAndDirection(EntityPlayerMP player);

  /**
   * Notifies listeners about the mission ending
   */
  protected void notifyAllAboutMissionEnding(String exitCode) {
    for (MissionListener listener : this.listeners) {
      listener.missionEnded(exitCode);
    }
  }

  /**
   * Save files and other pending stuff that should be flushed when the mission
   * ends. This method must be called by the subclasses upon the end of the
   * mission.
   */
  protected void cleanup() { this.saveSelfReports(); }

  /**
   * Save self-report content to a file
   * @param selfReportContent - Content of the self-report
   */
  private void saveSelfReports() {
    if (this.hasSelfReport()) {
      this.createSelfReportsFolder();
      for (MissionSelfReport selfReport : this.selfReportPerPlayer.values()) {
        if (selfReport.hasContent()) {
          String path = this.getSelfReportPath(selfReport);
          this.writeSelfReportToFile(path, selfReport);
        }
      }
    }
  }

  /**
   * Creates self-reports folder if it doesn't exist already
   */
  private void createSelfReportsFolder() {
    File folder = new File(SELF_REPORT_FOLDER);
    if (!folder.exists()) {
      folder.mkdir();
    }
  }

  /**
   * Get the filename for a given self-report based on some of its info
   */
  private String getSelfReportPath(MissionSelfReport selfReport) {
    String path = String.format("%s/self_report_player_%s.json",
                                SELF_REPORT_FOLDER,
                                selfReport.getPlayerID());
    return path;
  }

  /**
   * Write all self-reports from a player in a mission to a file
   * @param path - Filename of the self-report file
   * @param selfReport - Self-report object
   */
  private void writeSelfReportToFile(String path,
                                     MissionSelfReport selfReport) {
    try {
      FileWriter fileWriter = new FileWriter(path);
      Gson gson = new GsonBuilder().create();
      String json = gson.toJson(selfReport);
      fileWriter.write(json);
      fileWriter.close();
    }
    catch (IOException e) {
      e.printStackTrace();
    }
  }

  /**
   * Defines the duration of the mission in seconds
   * @param timeLimitInSeconds - Time in seconds until the end of the mission
   */
  public void setTimeLimitInSeconds(long timeLimitInSeconds) {
    this.timeLimitInSeconds = timeLimitInSeconds;
  }

  /**
   * Defines the time interval to wait until prompting a self-report screen to
   * the player
   * @param selfReportPromptTimeInSeconds - Time to wait until show a
   * self-report screen
   */
  public void
  setSelfReportPromptTimeInSeconds(long selfReportPromptTimeInSeconds) {
    this.selfReportPromptTimeInSeconds = selfReportPromptTimeInSeconds;
  }

  /**
   * Adds an entity to be deleted after certain amount of time
   * @param entity - Entity to be removed
   * @param delayInSeconds - Time (in seconds) to wait until removing the entity
   */
  public void addToDeletion(Entity entity, long worldTime) {
    this.entitiesToRemove.put(entity, worldTime);
  }
}
