#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;

int main()
{
  // read number of points
  std::size_t n;
  std::cin >> n;
  // construct triangulation
  Triangulation t;
  for (std::size_t i = 0; i < n; ++i) {
    Triangulation::Point p;
    std::cin >> p;
    t.insert(p);
  }
  // output all triangles
  for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
    std::cout << t.triangle(f) << "\n";
}
