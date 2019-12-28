#include "velvetg.h"


void velvetg(char *prefix) {
	ReadSet *sequences = NULL;
	RoadMapArray *rdmaps;
	PreGraph *preGraph;
	Graph *graph;
	char *directory, *graphFilename, *connectedGraphFilename,
	    *preGraphFilename, *seqFilename, *roadmapFilename,
	    *lowCovContigsFilename, *highCovContigsFilename;
	double coverageCutoff = -1;
	double longCoverageCutoff = -1;
	double maxCoverageCutoff = -1;
	double expectedCoverage = -1;
	Coordinate minContigLength = -1;
	Coordinate minContigKmerLength;
	boolean *dubious = NULL;
	Coordinate insertLength[CATEGORIES];
	Coordinate insertLengthLong = -1;
	Coordinate std_dev[CATEGORIES];
	Coordinate std_dev_long = -1;
	short int accelerationBits = 24;
	boolean readTracking = false;
	boolean exportAssembly = false;
	boolean unusedReads = false;
	boolean estimateCoverage = false;
	boolean estimateCutoff = false;
	boolean exportAlignments = false;
	FILE *file;
	ShortLength *sequenceLengths = NULL;
	Category cat;
	boolean scaffolding = true;
	int pebbleRounds = 1;
	boolean exportFilteredNodes = false;
	int clean = 0;
	boolean conserveLong = false;
	boolean shadows[CATEGORIES];
	int coverageMask = 1;
	SequencesReader *seqReadInfo = NULL;

	for (cat = 0; cat < CATEGORIES; cat++) {
		insertLength[cat] = -1;
		std_dev[cat] = -1;
		shadows[cat] = false;
	}

	// Memory allocation
	directory = prefix;
	graphFilename = mallocOrExit(strlen(directory) + 100, char);
	connectedGraphFilename = mallocOrExit(strlen(directory) + 100, char);
	preGraphFilename =
	    mallocOrExit(strlen(directory) + 100, char);
	roadmapFilename = mallocOrExit(strlen(directory) + 100, char);
	seqFilename = mallocOrExit(strlen(directory) + 100, char);
	lowCovContigsFilename = mallocOrExit(strlen(directory) + 100, char);
	highCovContigsFilename = mallocOrExit(strlen(directory) + 100, char);
	seqReadInfo = callocOrExit(1, SequencesReader);
	strcpy(seqFilename, directory);
	// if binary CnyUnifiedSeq exists, use it.  Otherwise try Sequences
	strcat(seqFilename, "/CnyUnifiedSeq");
	if (access(seqFilename, R_OK) == 0) {
		seqReadInfo->m_bIsBinary = true;
	} else {
		seqReadInfo->m_bIsBinary = false;
		strcpy(seqFilename, directory);
	strcat(seqFilename, "/Sequences");
	}
	seqReadInfo->m_seqFilename = seqFilename;
	strcpy(roadmapFilename, directory);
	strcat(roadmapFilename, "/Roadmaps");

	strcpy(preGraphFilename, directory);
	strcat(preGraphFilename, "/PreGraph");

	strcpy(connectedGraphFilename, directory);
	strcat(connectedGraphFilename, "/ConnectedGraph");

	if (!readTracking) {
		strcpy(graphFilename, directory);
		strcat(graphFilename, "/Graph");
	} else {
		strcpy(graphFilename, directory);
		strcat(graphFilename, "/Graph2");
	}

	strcpy(lowCovContigsFilename, directory);
	strcat(lowCovContigsFilename, "/lowCoverageContigs.fa");

	strcpy(highCovContigsFilename, directory);
	strcat(highCovContigsFilename, "/highCoverageContigs.fa");


	// Graph uploading or creation
	if ((file = fopen(graphFilename, "r")) != NULL) {
		fclose(file);

		graph = importGraph(graphFilename);

	} else if ((file = fopen(connectedGraphFilename, "r")) != NULL) {
		fclose(file);
		if (seqReadInfo->m_bIsBinary) {

			sequences = importCnyReadSet(seqFilename);

#if 0
			// compare to velvet's version of a seq
			ReadSet *compareSequences = NULL;
			compareSeqFilename = mallocOrExit(strlen(directory) + 100, char);
			strcpy(compareSeqFilename, directory);
			strcat(compareSeqFilename, "/Sequences");
			compareSequences = importReadSet(compareSeqFilename);
			convertSequences(compareSequences);
			if (sequences->readCount != compareSequences->readCount) {
				printf("read count mismatch\n");
				exit(1);
			}
			int i;
			for (i = 0; i < sequences->readCount; i++) {
				TightString *tString = getTightStringInArray(sequences->tSequences, i);
				TightString *tStringCmp = getTightStringInArray(compareSequences->tSequences, i);
				if (getLength(tString) != getLength(tStringCmp)) {
					printf("sequence %d len mismatch\n", i);
					exit(1);
				}
				if (strcmp(readTightString(tString), readTightString(tStringCmp)) != 0) {
					printf("sequence %d cmp mismatch\n", i);
					printf("seq %s != cmp %s\n", readTightString(tString), readTightString(tStringCmp));
					exit(1);
				}
			}
#endif
		} else {
			sequences = importReadSet(seqFilename);
			convertSequences(sequences);
		}
		seqReadInfo->m_sequences = sequences;

		graph =
		    importConnectedGraph(connectedGraphFilename, sequences,
				   roadmapFilename, readTracking, accelerationBits);

		sequenceLengths =
		    getSequenceLengths(sequences, getWordLength(graph));
		correctGraph(graph, sequenceLengths, sequences->categories, conserveLong);
		exportGraph(graphFilename, graph, sequences->tSequences);
	} else if ((file = fopen(preGraphFilename, "r")) != NULL) {
		fclose(file);
		if (seqReadInfo->m_bIsBinary) {
			sequences = importCnyReadSet(seqFilename);
		} else {
		sequences = importReadSet(seqFilename);
		convertSequences(sequences);
		}
		seqReadInfo->m_sequences = sequences;
		graph =
		    importPreGraph(preGraphFilename, sequences,
				   roadmapFilename, readTracking, accelerationBits);
		sequenceLengths =
		    getSequenceLengths(sequences, getWordLength(graph));
		correctGraph(graph, sequenceLengths, sequences->categories, conserveLong);
		exportGraph(graphFilename, graph, sequences->tSequences);
	} else if ((file = fopen(roadmapFilename, "r")) != NULL) {
		fclose(file);

		rdmaps = importRoadMapArray(roadmapFilename);
		if (seqReadInfo->m_bIsBinary) {
			// pull in sequences first and use in preGraph
			sequences = importCnyReadSet(seqFilename);
			seqReadInfo->m_sequences = sequences;
#if 0
			// compare to velvet's version of a seq
			ReadSet *compareSequences = NULL;
			char *compareSeqFilename = mallocOrExit(strlen(directory) + 100, char);
			strcpy(compareSeqFilename, directory);
			strcat(compareSeqFilename, "/Sequences");
			compareSequences = importReadSet(compareSeqFilename);
			convertSequences(compareSequences);
			if (sequences->readCount != compareSequences->readCount) {
				printf("read count mismatch\n");
				exit(1);
			}
			int i;
			for (i = 0; i < sequences->readCount; i++) {
				TightString *tString = getTightStringInArray(sequences->tSequences, i);
				TightString *tStringCmp = getTightStringInArray(compareSequences->tSequences, i);
				if (getLength(tString) != getLength(tStringCmp)) {
					printf("sequence %d len mismatch\n", i);
					exit(1);
				}
				if (strcmp(readTightString(tString), readTightString(tStringCmp)) != 0) {
					printf("sequence %d cmp mismatch\n", i);
					printf("seq %s != cmp %s\n", readTightString(tString), readTightString(tStringCmp));
					exit(1);
				}
			}
			printf("sequence files match!\n");
#endif
		}
		preGraph = newPreGraph_pg(rdmaps, seqReadInfo);
		concatenatePreGraph_pg(preGraph);
		if (!conserveLong)
		    clipTips_pg(preGraph);
		exportPreGraph_pg(preGraphFilename, preGraph);
		destroyPreGraph_pg(preGraph);
		if (!seqReadInfo->m_bIsBinary) {
		sequences = importReadSet(seqFilename);
		convertSequences(sequences);
			seqReadInfo->m_sequences = sequences;
		}
		graph =
		    importPreGraph(preGraphFilename, sequences,
				   roadmapFilename, readTracking, accelerationBits);
		sequenceLengths =
		    getSequenceLengths(sequences, getWordLength(graph));
		correctGraph(graph, sequenceLengths, sequences->categories, conserveLong);
		exportGraph(graphFilename, graph, sequences->tSequences);
	} else {
		velvetLog("No Roadmap file to build upon! Please run velveth (see manual)\n");
#ifdef DEBUG
		abort();
#endif
		exit(1);
	}

	// Set insert lengths and their standard deviations
	for (cat = 0; cat < CATEGORIES; cat++) {
		if (insertLength[cat] > -1 && std_dev[cat] < 0)
			std_dev[cat] = insertLength[cat] / 10;
		setInsertLengths(graph, cat,
				 insertLength[cat], std_dev[cat]);
	}

	if (insertLengthLong > -1 && std_dev_long < 0)
		std_dev_long = insertLengthLong / 10;
	setInsertLengths(graph, CATEGORIES,
			 insertLengthLong, std_dev_long);

	// Coverage cutoff
	if (expectedCoverage < 0 && estimateCoverage == true) {
		expectedCoverage = estimated_cov(graph, directory);
		if (coverageCutoff < 0) {
			coverageCutoff = expectedCoverage / 2;
			estimateCutoff = true;
		}
	} else {
		estimateCoverage = false;
		if (coverageCutoff < 0 && estimateCutoff)
			coverageCutoff = estimated_cov(graph, directory) / 2;
		else
			estimateCutoff = false;
	}

	if (coverageCutoff < 0) {
		velvetLog("WARNING: NO COVERAGE CUTOFF PROVIDED\n");
		velvetLog("Velvet will probably leave behind many detectable errors\n");
		velvetLog("See manual for instructions on how to set the coverage cutoff parameter\n");
	}

	if (sequences == NULL) {
		if (seqReadInfo->m_bIsBinary) {
			sequences = importCnyReadSet(seqFilename);
		} else {
		sequences = importReadSet(seqFilename);
		convertSequences(sequences);
	}
		seqReadInfo->m_sequences = sequences;
	}

	if (minContigLength < 2 * getWordLength(graph))
		minContigKmerLength = getWordLength(graph);
	else
		minContigKmerLength = minContigLength - getWordLength(graph) + 1;

	dubious =
	    removeLowCoverageNodesAndDenounceDubiousReads(graph,
							  coverageCutoff,
							  sequences,
							  exportFilteredNodes,
							  minContigKmerLength,
							  lowCovContigsFilename);

	removeLowLongCoverageNodesAndDenounceDubiousReads(graph,
							  longCoverageCutoff,
							  sequences,
							  dubious,
							  exportFilteredNodes,
							  minContigKmerLength,
							  lowCovContigsFilename);

	removeHighCoverageNodes(graph, maxCoverageCutoff, exportFilteredNodes, minContigKmerLength, highCovContigsFilename);
	clipTipsHard(graph, conserveLong);

	if (sequences->readCount > 0 && sequences->categories[0] == REFERENCE)
		removeLowArcs(graph, coverageCutoff);

	if (expectedCoverage > 0) {

		// Mixed length sequencing
		readCoherentGraph(graph, isUniqueSolexa, expectedCoverage,
				  sequences);

		// Paired end resolution
		createReadPairingArray(sequences);
		pebbleRounds += pairedCategories(sequences);
		detachDubiousReads(sequences, dubious);
		activateGapMarkers(graph);

		for ( ;pebbleRounds > 0; pebbleRounds--)
			exploitShortReadPairs(graph, sequences, dubious, shadows, scaffolding);

	} else {
		velvetLog("WARNING: NO EXPECTED COVERAGE PROVIDED\n");
		velvetLog("Velvet will be unable to resolve any repeats\n");
		velvetLog("See manual for instructions on how to set the expected coverage parameter\n");
	}

	if (dubious)
		free(dubious);

	concatenateGraph(graph);

	removeLowCoverageReferenceNodes(graph, coverageCutoff, longCoverageCutoff, sequences);

	strcpy(graphFilename, directory);
	strcat(graphFilename, "/contigs.fa");
	sequenceLengths = getSequenceLengths(sequences, getWordLength(graph));
	exportLongNodeSequences(graphFilename, graph, minContigKmerLength, sequences, sequenceLengths, coverageMask);

	if (exportAlignments) {
		strcpy(graphFilename, directory);
		strcat(graphFilename, "/contig-alignments.psa");
		exportLongNodeMappings(graphFilename, graph, sequences,
					     minContigKmerLength, seqReadInfo);
	}

	strcpy(graphFilename, directory);
	strcat(graphFilename, "/stats.txt");
	displayGeneralStatistics(graph, graphFilename, sequences);

	if (clean == 0) {
		strcpy(graphFilename, directory);
		strcat(graphFilename, "/LastGraph");
		exportGraph(graphFilename, graph, sequences->tSequences);
	}

	if (exportAssembly) {
		strcpy(graphFilename, directory);
		strcat(graphFilename, "/velvet_asm.afg");
		exportAMOSContigs(graphFilename, graph, minContigKmerLength, sequences);
	}

	if (unusedReads)
		exportUnusedReads(graph, sequences, minContigKmerLength, directory);

	if (estimateCoverage)
		velvetLog("Estimated Coverage = %f\n", expectedCoverage);
	if (estimateCutoff)
		velvetLog("Estimated Coverage cutoff = %f\n", coverageCutoff);

	logFinalStats(graph, minContigKmerLength, directory);

	if (clean > 0) {
		strcpy(graphFilename, directory);
		strcat(graphFilename, "/Roadmaps");
		remove(graphFilename);

		strcpy(graphFilename, directory);
		strcat(graphFilename, "/LastGraph");
		remove(graphFilename);
	}

	if (clean > 1) {
		strcpy(graphFilename, directory);
		strcat(graphFilename, "/Sequences");
		remove(graphFilename);

		strcpy(graphFilename, directory);
		strcat(graphFilename, "/Graph2");
		remove(graphFilename);

		strcpy(graphFilename, directory);
		strcat(graphFilename, "/Graph");
		remove(graphFilename);
	}

	free(sequenceLengths);
	destroyGraph(graph);
	free(graphFilename);
	free(connectedGraphFilename);
	free(preGraphFilename);
	free(seqFilename);
	free(roadmapFilename);
	free(lowCovContigsFilename);
	free(highCovContigsFilename);
	destroyReadSet(sequences);
	if (seqReadInfo) {
		free(seqReadInfo);
	}

}
