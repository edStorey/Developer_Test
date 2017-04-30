/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_20859823_INCLUDED
#define BINARYDATA_H_20859823_INCLUDED

namespace BinaryData
{
    extern const char*   cap_png;
    const int            cap_pngSize = 8008;

    extern const char*   highlightedselected2x_png;
    const int            highlightedselected2x_pngSize = 23631;

    extern const char*   normal2x_png;
    const int            normal2x_pngSize = 19349;

    extern const char*   off_png;
    const int            off_pngSize = 5138;

    extern const char*   on_png;
    const int            on_pngSize = 4872;

    extern const char*   selected2x_png;
    const int            selected2x_pngSize = 23608;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 6;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
