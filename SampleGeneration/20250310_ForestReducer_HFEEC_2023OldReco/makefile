default: TestRun

TestRun: Execute
	./Execute --Input /data/NewSkims23_24/20241102_ForestOldReco23sample_Dataexample/HiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
                  --Output SkimReco_UPCPbPb23_HiVertex_279.root --Year 2023 --IsData true \
                  --ApplyTriggerRejection 0 \
                  --ApplyEventRejection false \
                  --ApplyZDCGapRejection false \
                  --PFTree particleFlowAnalyser/pftree

Execute: ReduceForest.cpp
	g++ ReduceForest.cpp -o Execute \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
