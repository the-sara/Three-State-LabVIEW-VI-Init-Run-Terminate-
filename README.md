# Three-State-LabVIEW-VI-Init-Run-Terminate-
This VI has three states:  INIT: runs only at the start  RUN: loops while the VI is active  TERMINATE: runs once when the VI stops  If the user presses Abort Execution, the loop stops instantly and the VI goes to TERMINATE to perform cleanup before closing.
