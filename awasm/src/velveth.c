#include "velveth.h"


// Expand kseq macros to define functions with custom buffer size
KSTREAM_INIT(struct FileReader, read_data, BUFFER_SZ)
__KSEQ_TYPE(struct FileReader)
__KSEQ_BASIC(static, struct FileReader)
__KSEQ_READ(static)


void velveth(char *prefix) {
  ReadSet *allSequences = NULL;
  SplayTable *splayTable;
  int hashLength, hashLengthStep, hashLengthMax, h;
  char *directory, *filename, *seqFilename, *baseSeqName, *buf;
  boolean double_strand = true;
  boolean multiple_kmers = false;
  DIR *dir;

  multiple_kmers = false;

  hashLength = 31;
  hashLengthMax = hashLength + 1;
  hashLengthStep = 2;

  for (h = hashLength; h < hashLengthMax; h += hashLengthStep) {
    resetWordFilter(h);
    buf = mallocOrExit(5120, char);
    if (multiple_kmers) {
      fprintf(stderr, "error: multiple kmers not yet implemented\n");
      exit(0);

      // TODO: webasm-ify
      //printf("command: %s_%d",argv[1],h);
      //sprintf(buf,"%s_%d",argv[1],h);
      //printf("malloc\n");
      //directory = mallocOrExit(strlen(buf) + 100, char);
      //strcpy(directory,buf);
    } else {
      directory = prefix;
    }

    filename = mallocOrExit(strlen(directory) + 100, char);
    seqFilename = mallocOrExit(strlen(directory) + 100, char);
    baseSeqName = mallocOrExit(100, char);

    dir = opendir(directory);

    if (dir == NULL) {
      mkdir(directory, 0777);
    } else {
      //fprintf(stderr, "error: dir create not yet implemented\n");
      //exit(0);

      // TODO: webasm-ify
      // TODO: implement recursive directory deletion
      //sprintf(buf, "%s/PreGraph", directory);
      //remove(buf);
      //sprintf(buf, "%s/Graph", directory);
      //remove(buf);
      //sprintf(buf, "%s/Graph2", directory);
      //remove(buf);
      //sprintf(buf, "%s/Graph3", directory);
      //remove(buf);
      //sprintf(buf, "%s/Graph4", directory);
      //remove(buf);
    }

    strcpy(seqFilename, directory);
    strcpy(baseSeqName, "/Sequences");
    // TODO: webasm-ify
    // Delete directory/CnyUnifiedSeq
    //sprintf(buf, "%s/CnyUnifiedSeq", directory);
    //remove(buf);
    // Delete directory/CnyUnifiedSeq.names
    //sprintf(buf, "%s/CnyUnifiedSeq.names", directory);
    //remove(buf);
    strcat(seqFilename, baseSeqName);

    if (h == hashLength) {
      // Load data on first hash
      SequencesWriter * seqWriteInfo = NULL;
      Category cat = 1;  // shortPaired
      IDnum sequenceIndex = 1;

      seqWriteInfo = callocOrExit(1, SequencesWriter);
      seqWriteInfo->m_pFile = fopen(seqFilename, "w");

      initFastX(seqWriteInfo, cat);

      struct FileReader *seq1_fr = init_file_reader("seq1", BUFFER_SZ);
      struct FileReader *seq2_fr = init_file_reader("seq2", BUFFER_SZ);
      kseq_t *seq1 = kseq_init(*seq1_fr);
      kseq_t *seq2 = kseq_init(*seq2_fr);

      size_t i = 0;
      IDnum counter = 0;
      while (kseq_read(seq1) >= 0) {
        i++;
        counter++;
        writeSeqName(seq1->name.s, seqWriteInfo, cat, &sequenceIndex);
        writeSequence(seq1->seq.s, seqWriteInfo);

        // TODO: current checks do not work, fix
        //if (kseq_read(seq2) < 0) {
        //  fprintf(stderr, "Second sequence file has too few sequences");
        //  exit(1);
        //}
        kseq_read(seq2);

        counter++;
        writeSeqName(seq2->name.s, seqWriteInfo, cat, &sequenceIndex);
        writeSequence(seq2->seq.s, seqWriteInfo);
      }
      // TODO: current checks do not work, fix
      //if (kseq_read(seq2) >= 0) {
      //    fprintf(stderr, "First sequence file has too few sequences");
      //    exit(1);
      //}

      kseq_destroy(seq1);
      kseq_destroy(seq2);

      cleanupFastX(seqWriteInfo, cat);

      fclose(seqWriteInfo->m_pFile);
    } else {
      fprintf(stderr, "error: multiple kmers not yet implemented\n");
      exit(0);

      // TODO: webasm-ify
      // Remove seqfilename, then symlink for next hash iteration
      //sprintf(buf,"rm -f %s",seqFilename);
      //sprintf(buf,"ln -s `pwd`/%s_%d%s %s",argv[1],hashLength,baseSeqName,seqFilename);
    }

    splayTable = newSplayTable(h, double_strand);
    allSequences = importReadSet(seqFilename);
    //printf("%li sequences in total.\n", (long) allSequences->readCount);

    strcpy(filename, directory);
    strcat(filename, "/Roadmaps");
    inputSequenceArrayIntoSplayTableAndArchive(allSequences, splayTable, filename, seqFilename);

    destroySplayTable(splayTable);
    if (dir)
      closedir(dir);
    if (directory != prefix)
      free(directory);
    free(filename);
    free(seqFilename);
    free(baseSeqName);
    free(buf);
    if (allSequences) {
      destroyReadSet(allSequences);
    }
  }
}
