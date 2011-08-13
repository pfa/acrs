/* acrs4.hpp */

#ifndef ACRS4_H
#define ACRS4_H

#include <iostream>
#include <string>
#include <list>

#include "ip4route.hpp"

namespace Acrs4
{
    bool overlapCmp(IP4Route::IP4Route & rt1, IP4Route::IP4Route & rt2);
    bool acrsCmp(IP4Route::IP4Route & rt1, IP4Route::IP4Route & rt2);

    class Acrs4 : public std::list<IP4Route::IP4Route>
    {
    private:
        bool m_logging;
        std::ostream & m_os;
        int m_main_recurse_count;

        bool summarizeOverlap(void);
        bool summarizeMain(void);
        void log(const std::string & msg) const;

    public:
        void setLogging(bool logging) { m_logging = logging; };
        bool getLogging(void) { return m_logging; };
        bool summarize(void);

        /* Constructor */
        Acrs4(std::ostream & os = std::cout, bool logging = false) :
                                             m_os(os),
                                             m_logging(logging) {} ;

        virtual ~Acrs4(void) {};
    };
}

#endif /* ACRS4_H */
