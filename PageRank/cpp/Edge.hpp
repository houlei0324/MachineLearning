#ifndef EDGE_HPP_
#define EDGE_HPP_

typedef int VertexID;
typedef double ED;
class Edge{
	public:
		Edge(){}
		Edge(VertexID _src, VertexID _dst, ED _attr): src(_src), dst(_dst), attr(_attr) {}
	public:
		VertexID src, dst;
		ED attr;
	public:
		VertexID getSrc() const {
			return src;
		}
		VertexID getDst() const {
			return dst;
		}
		ED getAttr() const {
			return attr;
		}
};

#endif /*EDGE_HPP_*/
