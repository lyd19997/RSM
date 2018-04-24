#include"request.h"

Request::Request(int id_, int src_, int dst_, int start_, int end_, double value_, double rate_) :id(id_), src(src_), dst(dst_), start(start_), end(end_), value(value_), rate(rate_) {
	if (rate > 32) rate = 32;//wakaka
} 

pair<int, int> Request::getSrcDst() {
	return pair<int, int>(src, dst);
}



