#!/bin/bash
DATE=$(date +%Y%m%d)

# Source of forest files (use xrootd path)
SOURCE_SERVER="root://xrootd.cmsaf.mit.edu/"
SOURCE_DIR="/store/user/jdlang/run3_2024PromptReco"
# Output of skimmed files (use xrootd path)
OUTPUT_SERVER="root://xrootd.cmsaf.mit.edu/"
OUTPUT_DIR="/store/user/$USER/run3_2023Data_Nov2024PromptReco_Skims_$DATE"

# Job settings (memory and storage are in GB)
FILES_PER_JOB=200
JOB_MEMORY=5
JOB_STORAGE=20
CMSSW_VERSION="CMSSW_14_1_7"

# Local directory for condor configs
CONFIG_DIR="condorSkimConfigs_${DATE}"
MASTER_FILE_LIST="${CONFIG_DIR}/forestFilesForSkim.txt"
# Include VOMS proxy in process
REFRESH_PROXY=1

$ProjectBase/SampleGeneration/20241203_CondorSkimUtils/InitCondorSkim.sh $SOURCE_SERVER $SOURCE_DIR $OUTPUT_SERVER $OUTPUT_DIR $FILES_PER_JOB $JOB_MEMORY $JOB_STORAGE $CMSSW_VERSION $CONFIG_DIR $MASTER_FILE_LIST $REFRESH_PROXY
