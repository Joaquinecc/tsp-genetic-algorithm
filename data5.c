#include "data5.h"
 int map[V][V] = { 
				//			A			B		C		 D			E		
				/*A*/	{ 	0	,		2,	 INFINITE,	 12,		 5 },
				/*B*/	{ 	2,	 		0, 		4, 		  8,	 INFINITE },
				/*C*/	{ INFINITE,     4, 	 	0, 	  	  3,		3 },
				/*D*/	{ 	12, 		8, 		3, 		  0,		10 },
				/*E*/	{ 	5,		INFINITE, 	3, 		 10, 		0} 
				};
				
// Names of the cities
 char CITIES[]="ABCDE";