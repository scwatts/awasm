#ifndef __VELVETH_H__
#define __VELVETH_H__


#include <dirent.h>
#include <sys/stat.h>


#include "zlib.h"
#include "kseq.h"


#include "velvet/src/run.h"
#include "velvet/src/autoOpen.h"


#include "disk_streamer.h"


// Defined in velvet/src/readSet.c
typedef struct {
  gzFile gzFile;
  AutoFile *autoFile;
} FileGZOrAuto;


// Defined in velvet/src/readSet.c
extern void writeSeqName(char*seq_name, SequencesWriter *seqWriteInfo, Category cat, IDnum *sequenceIndex);
extern void writeSequence(char*seq, SequencesWriter *seqWriteInfo);
extern void initFastX(SequencesWriter *seqWriteInfo, Category cat);
extern void cleanupFastX(SequencesWriter *seqWriteInfo, Category cat);


void velveth(char *prefix);


#endif
