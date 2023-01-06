//
// Created by Laure on 2022-12-13.
//

#ifndef AOCC_DAY13_FUNCTIONS_H
#define AOCC_DAY13_FUNCTIONS_H

#include "../../library/pointerList.h"

int isPointerList(PointerList *list, int index);

PointerList *parseInput();

int packetComparator(void *one, void *two);

void deletePackets(PointerList *packets);

#endif //AOCC_DAY13_FUNCTIONS_H
