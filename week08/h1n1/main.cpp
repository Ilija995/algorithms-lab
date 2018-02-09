#include <iostream>
#include <vector>
#include <stack>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<bool,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;



void testcase(int n)
{
    std::vector<K::Point_2> ppl;
    ppl.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        ppl.emplace_back();
        std::cin >> ppl.back();
    }

    Triangulation t;
    t.insert(ppl.begin(), ppl.end());

    int m;
    std::cin >> m;
    //std::cout << m << std::endl;
    K::Point_2 p;
    int d;
    for (int i = 0; i < m; ++i)
    {
        std::cin >> p >> d;
        auto vh = t.nearest_vertex(p);
        //std::cout << "[" << vh->point() << "] <- [" << p << "] = " << CGAL::squared_distance(vh->point(), p) << std::endl;
        if (CGAL::squared_distance(vh->point(), p) < d)
        {
            std::cout << "n";
            continue;
        }

        for (Triangulation::All_faces_iterator fi = t.all_faces_begin(); fi != t.all_faces_end(); ++fi)
            fi->info() = false;

        std::stack<Triangulation::Face_handle> s;
        auto fh = t.locate(p);
        fh->info() = true;
        s.push(fh);
        bool escaped = false;
        while (!s.empty())
        {
            fh = s.top();
            s.pop();

            if (t.is_infinite(fh))
            {
                escaped = true;
                break;
            }

            for (int j = 0; j < 3; ++j)
            {/*
                std::cout << "adding - ["
                            << fh->vertex(j)->point() << "] <- ["
                            << fh->vertex((j + 1) % 3)->point() << "] = "
                            << CGAL::squared_distance(fh->vertex(j)->point(), fh->vertex((j + 1) % 3)->point())
                            << std::endl;*/
                if (!fh->neighbor((j + 2) % 3)->info() && CGAL::squared_distance(fh->vertex(j)->point(), fh->vertex((j + 1) % 3)->point()) >= 4 * d)
                {
                    //std::cout << "added" << std::endl;
                    fh->neighbor((j + 2) % 3)->info() = true;
                    s.push(fh->neighbor((j + 2) % 3));
                }
            }
        }

        std::cout << ((escaped) ? "y" : "n");
    }

    std::cout << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    //int t = 0;
    int n;
    std::cin >> n;
    while (n > 0)
    {
        //std::cout<< t++ << std::endl;
        testcase(n);
        std::cin >> n;
    }
    return 0;
}
