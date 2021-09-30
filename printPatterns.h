#pragma once

#include "printer.h"

const int toPrint = 0
    | Printer::COUNTER
    // | Printer::INITIALS
    // | Printer::STARTING_DNA
    // | Printer::ORIGINAL_OLIGOS
    // | Printer::ORIGINAL_LOCATIONS
    // | Printer::ORIGINAL_OLIGOS_WITH_LOCATIONS
    // | Printer::POSITIVE_ERRORS
    // | Printer::NEGATIVE_ERRORS
    // | Printer::WITH_ERRORS_OLIGOS
    // | Printer::WITH_ERRORS_LOCATIONS
    // | Printer::WITH_ERRORS_OLIGOS_WITH_LOCATIONS
    // | Printer::GRAPH
    | Printer::RESULTS_WITHOUT_DNA_STRING
    // | Printer::RESULTS_GREEDY
    // | Printer::RESULTS_AFTER_COMPACTION
    // | Printer::RESULTS_AFTER_LENGTHENING
    // | Printer::RESULTS_FINAL
    | Printer::RESULTS_GLOBAL_FINAL
    ;