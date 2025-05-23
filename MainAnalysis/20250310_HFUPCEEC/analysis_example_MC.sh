TrackPTMIN=0.2
TrackPTMAX=1000000
TrackYMIN=-2.4
TrackYMAX=+2.4
EvisMIN=0.0
EvisMAX=10000
MvisMIN=0.0
MvisMAX=10000
USEEVisHardScale=1
TRIGGER=0 # no trigger selection
ISGAMMAN=0
ISDATA=0
SCALEFACTOR=1.0
#INPUT=/data/HFJetUPCanalysis/MCSkims23/20250310_ForestHFJetMC23_D0Tagged_v9_ModTrackCuts_pthat5_BeamAabdBCombined.root
INPUT=/data/HFJetUPCanalysis/MCSkims23/20250514_ForestHFJetMC23_D0Tagged_v1_pthat0.root
OUTPUTANALYSIS=output_MC_pthat0_BeamAandBcombined_TrackPtCut020__Evis10_20_v4.root


./ExecuteHFUPCEEC --Input $INPUT --MinTrackPT $TrackPTMIN --MaxTrackPT $TrackPTMAX --MinTrackY $TrackYMIN --MaxTrackY $TrackYMAX --MinEvis $EvisMIN --MaxEvis $EvisMAX --MinMvis $MvisMIN --MaxMvis $MvisMAX --UseEvisHardScale $USEEVisHardScale  --IsGammaN $ISGAMMAN --TriggerChoice $TRIGGER --IsData $ISDATA --Output $OUTPUTANALYSIS --scaleFactor $SCALEFACTOR
wait
