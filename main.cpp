#include <iostream>
#include <fstream>
#include <random>
#include <functional>
#include <sys/time.h>

#include "dynamic_k_reach.h"
#include "dynamic_k_reach_v2.h"

using namespace std;

double GetCurrentTimeSec()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

int main()
{
    vector<edge_t> edges;
    vertex_t max_id = 0;
    {
        string filename("lastfm");
        ifstream fin(filename);
        for (vertex_t s, t; fin >> s >> t;) {
            edges.push_back(make_pair(s, t));
            max_id = max(max_id, max(s, t));
        }
    }

    dynamic_k_reach_v2 dkr1, dkr2;
    dkr1.construct_index(edges, 3);
    cout << "Constructed 1" << endl;
    dkr2.construct_index(edges, 3);
    cout << "Constructed 2" << endl;

    {
        cout << "Before removal:" << endl;
        auto out_1 = dkr1.getOut_index(), out_2 = dkr2.getOut_index(), in_1 = dkr1.getIn_index(), in_2 = dkr2.getIn_index();
        cout << "out1 == out2: " << (out_1 == out_2) << endl;
        cout << "in1 == in2: " << (in_1 == in_2) << endl;
    }

    auto rand_edge = bind(uniform_int_distribution<size_t>(0, edges.size() - 1), default_random_engine());

    for (uint8_t i = 0; i < 100; ++i){
        size_t e = rand_edge();

        cout << "Random edge: " << edges[e].first << " " << edges[e].second << endl;
        double rem = -GetCurrentTimeSec();
        dkr1.remove_edge(edges[e].first, edges[e].second);
        rem += GetCurrentTimeSec();
        cout << "Removed 1 in " << rem << endl;
        double rem_rei = -GetCurrentTimeSec();
        dkr2.remove_edge_reindex(edges[e].first, edges[e].second);
        rem_rei += GetCurrentTimeSec();
        cout << "Removed 2 in " << rem_rei << endl;
        cout << "Speedup: " << rem_rei / rem << endl;

        {
            cout << "After removal:" << endl;
            auto out_1 = dkr1.getOut_index(), out_2 = dkr2.getOut_index(), in_1 = dkr1.getIn_index(), in_2 = dkr2.getIn_index();
            bool out = out_1 == out_2, in = in_1 == in_2;
            cout << "out1 == out2: " << out << endl;
            cout << "in1 == in2: " << in << endl;
            if (!out || !in){
                cout << "ERROR" << endl;
                break;
            }
        }
    }

    /*auto rand_vtx = bind(uniform_int_distribution<vertex_t>(0, max_id), default_random_engine());

    for (uint8_t i = 0; i < 100; ++i) {
        vertex_t s = rand_vtx(), t = rand_vtx();
        cout << "Random edge: " << s << " " << t << endl;
        double ins = -GetCurrentTimeSec();
        dkr1.insert_edge(s, t);
        ins += GetCurrentTimeSec();
        cout << "Inserted 1 in " << ins << endl;
        double ins_rei = -GetCurrentTimeSec();
        dkr2.insert_edge_reindex(s, t);
        ins_rei += GetCurrentTimeSec();
        cout << "Inserted 2 in " << ins_rei << endl;
        cout << "Speedup: " << ins_rei / ins << endl;

        {
            cout << "After insertion:" << endl;
            auto out_1 = dkr1.getOut_index(), out_2 = dkr2.getOut_index(), in_1 = dkr1.getIn_index(), in_2 = dkr2.getIn_index();
            bool out = out_1 == out_2, in = in_1 == in_2;
            cout << "out1 == out2: " << out << endl;
            cout << "in1 == in2: " << in << endl;
            if (!out || !in){
                cout << "ERROR" << endl;
                break;
            }
        }
    }*/
//    cout << "Index constructed" << endl;

    /*{
        ifstream fin("dels_e");
        vertex_t s, t;
        while (fin >> s >> t){
            dkr.remove_edge(s, t);
            cout << "Edge removed" << endl;
        }
        fin.close();
    }*/

    /*{
        ifstream fin("dels_v_er");
        vertex_t v;
        while (fin >> v){
            dkr.remove_vertex(v);
            cout << "Vertex removed" << endl;
        }
        fin.close();
    }

    {
        ifstream fin("queries_er");
        vertex_t s, t;
        while (fin >> s >> t){
            cout << s << " " << t << ": " << dkr.query_reachability(s, t) << endl;
        }
        fin.close();
    }*/

    return 0;
}