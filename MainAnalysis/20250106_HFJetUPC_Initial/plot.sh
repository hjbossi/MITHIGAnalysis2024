INPUT=output_MC_JetPt_SubleadingJet.root

./ExecutePlotHFJet --Input $INPUT --tag "SubleadingJet"

./ExecutePlotDoFragFunction --Input "output_MC_AllJets.root","output_MC_JetPt_5_10.root","output_MC_JetPt_10_20.root","output_MC_JetPt_20_30.root","output_MC_JetPt_30_50.root","output_MC_JetPt_50_100.root" --tag "MCSecondLook"