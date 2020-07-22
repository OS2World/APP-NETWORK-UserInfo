/*      function  uinfo           Get user information

        purpose   To retrieve the domain name, userid, computername,
                  and user comment for the current network user.

        note      This program is set up as both a standalone program
                  and a function.  The default when compiling is to
                  generate a function, to compile for standalone use
                  define the macro variable MPROG.

        arguments

        name                  type    i/o   description
        domain                char**  O     A pointer to the newly
                                            allocated buffer containing
                                            the name of the domain.
        machine               char**  O     A pointer to the newly
                                            allocated buffer containing
                                            the name of the local computer.
        userid                char**  O     A pointer to the newly
                                            allocated buffer containing
                                            the current userid.
        username              char**  O     A pointer to the newly
                                            allocated buffer containing
                                            the user description field
                                            (usually their name).
*/
#define INCL_BASE
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <neterr.h>
#include <netcons.h>
#include <wksta.h>
#include <access.h>
#include <util.h>


#if defined(MPROG)
int main ()
  {
    char        *domain;
    char        *machine;
    char        *userid;
    char        *username;
#else
#define   domain (*domain_)
#define   machine (*machine_)
#define   userid (*userid_)
#define   username (*username_)
int uinfo (char **domain_,
           char **machine_,
           char **userid_,
           char **username_)
  {
#endif
    int         rc=0;
    SHORT       level;
    USHORT      buflen;
    USHORT      totalavail;
    CHAR        *servername;
    CHAR far    *buf;
    struct wksta_info_10 far *info_a;
    struct user_info_2 far *info_b;

    servername = "\0";
    level = 10;
    buflen = 0;

    rc = NetWkstaGetInfo (servername, level, buf, buflen, &totalavail);
    buflen = totalavail;
    buf = (CHAR far *) malloc (buflen);
    rc = NetWkstaGetInfo (servername, level, buf, buflen, &totalavail);
    info_a = (struct wksta_info_10 far *) buf;
    domain = strdup (info_a->wki10_logon_domain);
    machine= strdup (info_a->wki10_computername);
    userid = strdup (info_a->wki10_username);
#if defined(MPROG)
    printf ("   Domain: \"%s\"\n", domain);
    printf ("  Machine: \"%s\"\n", machine);
    printf ("   Userid: \"%s\"\n", userid);
#endif
    buflen = 0;
    level = 2;
    rc = NetUserGetInfo (servername,
                         userid,
                         level,
                         buf,
                         buflen,
                         &totalavail);
    buflen = totalavail;
    buf = realloc (buf, buflen);
    rc = NetUserGetInfo (servername,
                         userid,
                         level,
                         buf,
                         buflen,
                         &totalavail);
    info_b = (struct user_info_2 far *) buf;
    username = strdup (info_b->usri2_usr_comment);

#if defined(MPROG)
    printf ("User name: \"%s\"\n", username);
#endif

#if defined(MPROG)
    exit (rc);
#else
    return (rc);
#endif
  }
