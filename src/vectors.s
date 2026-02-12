.import IntReset, IntNMI, IntIRQ

.export Vector_Table

.segment "VECTORS"
Vector_Table:
    .addr IntNMI
    .addr IntReset
    .addr IntIRQ