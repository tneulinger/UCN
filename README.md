--------------------------------------------------------------------------------
  UCNTrackingAction:
--------------------------------------------------------------------------------
  - Calculate time that UCN was in simulation: "alive time":
    * Get time of "birth" in PreUserTrackingAction.
    * Get time of "death" in PostUserTrackingAction.
    * Output difference in seconds to a file.
