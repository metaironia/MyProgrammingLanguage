#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <stdint.h>

#include "functions_for_input.h"
#include "functions_for_output.h"
#include "strings_sort.h"

int main (void) {

// ------------------------------------ Inputting part -----------------------------------------------------

    FILE *onegin = fopen (FILE_NAME, "rb");                      //TODO fopen to fclose do func

    assert (onegin);

    struct stat onegin_stat = {};        //TODO func
    stat (FILE_NAME, &onegin_stat);

    char *buf = BufferMaker (onegin_stat.st_size);

    assert (buf);

    FileToBuf (onegin, buf, onegin_stat.st_size);

    fclose (onegin);

    int64_t number_of_strings = StringCounter (buf, onegin_stat.st_size);

    PtrToStr *pointers_to_strings = PointersToStringsMaker (number_of_strings);

    assert (pointers_to_strings);

    SetPointersToBuf (pointers_to_strings, buf, onegin_stat.st_size, number_of_strings);

    SlashRSymbolChecker (pointers_to_strings, number_of_strings);

    FILE *evgenin = fopen ("evgenin.txt", "w");

// ------------------------------------ Straight  sort -------------------------------------------------------

    StringsSort (pointers_to_strings, number_of_strings, sizeof (PtrToStr), StringForwardComparator);

    PrintAll (evgenin, pointers_to_strings, number_of_strings);

    PrintLine (evgenin);

// ------------------------------------ Backwards sort -------------------------------------------------------

    StringsSort (pointers_to_strings, number_of_strings, sizeof (PtrToStr), StringBackwardComparator);

    PrintAll (evgenin, pointers_to_strings, number_of_strings);

    PrintLine (evgenin);

// ------------------------------------ No sort --------------------------------------------------------------

    PrintOriginal (evgenin, buf, onegin_stat.st_size);

// ------------------------------------ Closing part ---------------------------------------------------------

    fclose (evgenin);
    evgenin = NULL;

    free (buf);
    buf = NULL;

    return 0;
}

//TODO find out how ptr to file works
