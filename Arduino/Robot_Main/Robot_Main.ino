Swarm_Bot robot;

void setup() {
  //Startup Code:
  //Set port directions
  //Initialize variables
  robot.init();

  //Connect to server via bluetooth
  robot.connectToServer();

  //Establish sequential robot IDs
  //based on bluetooth addresses
  robot.addNeighbors(robot.getNeighbors(), robot.getNumberOfNeighbors());
  robot.setID(robot.findIDFromNeighbors());

  //Assign robot role based on ID
  if (robot.ID == 0){
    robot.setRole(MAPPING);
  }
  else{
    robot.setRole(BEACON); 
  }
}

void loop() {
  if (robot.getRole() == MAPPING){
    //Territory is unexplored?
    if (!robot.get_mission_map().isExplored()){
      //Move certain distance forward
      robot.moveForward(DEFAULT_TIME, robot.findOpenDirection(DEFAULT_DISTANCE));

      //Turn on IR LED
      //Transmit ultrasonic chirp
      robot.sendChirp();

      //Get distance data from all neighbors
      Neighbor closestNeighbors[] = robot.getClosestNeighbors();
      BeaconData beacons[MIN_NUM_BEACONS];

      //For all of the neighbors
      for (int i=0; i < MIN_NUM_BEACONS; i++){
        //Send chirp and wait for distance data from neighbor
        beacons[i] = robot.findDistanceToNeighbor(closestNeighbors[i]);
      }

      //Calculate current position
      robot.setPosition(robot.trilaterate(beacons, MIN_NUM_BEACONS));

      //Measure Distance from walls
      robot.collectObstacleData();

      //Transmit current position,
      //wall distances, and compass
      //heading to server via bluetooth
      robot.updateServer();

      //Pass the flag to another robot
      robot.setNewRoamer(robot.getFarthestNeighbor);

      //Become Beacon
      robot.setRole(BEACON);
    }
    else{

    }
  }
  else{
    //Wait for IR signal
    while(robot.waitingForIR());
    //Store Time
    unsigned long irTime = millis(); 

    //Ultrasonic chirp received?
    while(robot.waitingForUltrasound()){
      if (millis - irTime > CHIRP_TIMEOUT){
        break;
      } 
      else{
        //Store time
        unsigned long usTime = millis();
      }
    }

    //Calculate distance based on time elapsed
    //Broadcast distance via Bluetooth
    robot.sendDistanceToRoamer(robot.calculateDistance(usTime - irTime));
    
    for (int i=0; i < REASSIGN_TIMEOUT; i++){
       if (robot.becomeNewRoamer()){
          robot.setRole(MAPPING);
          break;
       }
       else{
         delay(1000);
       }
    }
  }
}




