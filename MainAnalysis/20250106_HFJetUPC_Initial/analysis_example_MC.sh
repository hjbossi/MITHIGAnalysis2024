JETPTMIN=0
JETPTMAX=10000
JETYMIN=-2.4
JETYMAX=+2.4
DZEROPTMIN=2.0
DZEROPTMAX=100000
DZEROYMIN=-2.4
DZEROYMAX=2.4
TRIGGER=0 # no trigger selection
ISGAMMAN=0
ISDATA=0
SCALEFACTOR=1.0
INPUT=/data/HFJetUPCanalysis/MCSkims23/20250114_ForestHFJetMC23_D0Tagged_v2.root
OUTPUTANALYSIS=output_MC_JetPt_SubleadingJet.root


./ExecuteHFJetUPC --Input $INPUT --MinDzeroPT $DZEROPTMIN --MaxDzeroPT $DZEROPTMAX --MinDzeroY $DZEROYMIN --MaxDzeroY $DZEROYMAX --MinJetPT $JETPTMIN --MaxJetPT $JETPTMAX --MinJetY $JETYMIN --MaxJetY $JETYMAX --IsGammaN $ISGAMMAN --TriggerChoice $TRIGGER --IsData $ISDATA --Output $OUTPUTANALYSIS --scaleFactor $SCALEFACTOR
wait
