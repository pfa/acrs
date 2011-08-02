/* acrs4.hpp */

#ifndef ACRS4_H
#define ACRS4_H

#include <iostream>
#include <string>
#include <list>

#include "ip4route.hpp"

namespace Acrs4 {
    bool acrsCmp(IP4Route::IP4Route & rt1, IP4Route::IP4Route & rt2);
    bool overlapCmp(IP4Route::IP4Route & rt1, IP4Route::IP4Route & rt2);
    bool Summarize(std::list<IP4Route::IP4Route> & rtlist, bool logging,
                   std::ostream & os);
    bool SummarizeOverlap(std::list<IP4Route::IP4Route> & rtlist, bool logging,
                          std::ostream & os);
}

#endif /* ACRS4_H */
