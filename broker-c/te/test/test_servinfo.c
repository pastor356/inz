#include "serv_info.h"


void test_servinfo(void) {
	serv_info_t* si = create_servinfo();
	init_servinfo(si, "myserv");

	push_back_si(si, "newtopic", 3, 2);


	push_back_si(si, "newnwetopic", 3, 3);
	push_back_si(si, "newnwetopic", 3, 3);
	push_back_si(si, "newnwetopic", 3, 3);
	push_back_si(si, "newnwetopic", 3, 3);
	push_back_si(si, "newnwetopic", 3, 3);
	push_back_si(si, "newnwetopic", 3, 3);




	destroy_servinfo(si);
	return;
}


