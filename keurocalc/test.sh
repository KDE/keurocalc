# !/bin/bash
# This program asks KEuroCalc to compute "2 + 3"

SERVICE=$(qdbus | grep keurocalc)

qdbus $SERVICE /KEuroCalc

qdbus $SERVICE /KEuroCalc InputTwo
qdbus $SERVICE /KEuroCalc InputPlus
qdbus $SERVICE /KEuroCalc InputThree
qdbus $SERVICE /KEuroCalc ValidateSimpleValue
