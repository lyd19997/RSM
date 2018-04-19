#include"request.h"

Request::Request(int src_, int dst_, int start_, int end_, int value_, double rate_):src(src_),dst(dst_),start(start_),end(end_),value(value_),rate(rate_) {

}

pair<int, int> Request::getSrcDst() {
	return pair<int, int>(src, dst);
}
