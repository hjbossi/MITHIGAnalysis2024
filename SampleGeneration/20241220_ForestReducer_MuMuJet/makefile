
default: TestRun

TestRun: Execute
	./Execute --Input /data00/g2ccbar/mc2018/forest_12172024/HiForestMiniAOD_1-236.root --IsData true --IsPP false --Output SkimReco.root --MinJetPT 80 --Fraction 1.00

TestRunBase: Execute
	./Execute --Input /data00/g2ccbar/mc2018/forest_12172024/HiForestMiniAOD_1-236.root --IsData true --IsPP false --Output SkimReco.root --MinJetPT 80 --Fraction 1.00

Execute: ReduceForest.cpp
	g++ ReduceForest.cpp -o Execute \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
