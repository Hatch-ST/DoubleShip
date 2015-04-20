#ifndef INCLUDED_GAMELIB_DELETE_H
#define INCLUDED_GAMELIB_DELETE_H

#ifndef NULL
#define NULL 0
#endif

#define SAFE_DELETE( p ){ if ( p ){ delete ( p ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ){ if ( p ){ delete[] ( p ); ( p ) = NULL; } }
#define SAFE_RELEASE( p ){ if ( p && ( p )->Release() == 0 ){ ( p ) = NULL; } }

#endif