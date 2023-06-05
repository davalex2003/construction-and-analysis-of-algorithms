// АиСД - 2, 2023, задание 5
// Давиденко Алексей Ильич, БПИ214
// PyCharm + Clion + Jupyter Notebook
// Сгенерированы графики всех видов и размеров, реализованы алгоритмы поиска, проведены замеры по времени, построены графики, написан отчёт

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <set>
#include <string>
#include <map>

#define LIM 1e9

struct Edge {
    int a, b, cost;
};

void dijkstra(std::vector<std::vector<std::pair<int, int>>> g, int n) {
    std::vector<int64_t> d(n, LLONG_MAX);
    int v;
    d[0] = 0;
    std::set<std::pair<int, int>> q;
    q.insert(std::make_pair(0, 0));
    while (!q.empty()) {
        v = q.begin()->second;
        q.erase(q.begin());
        for (auto &j: g[v]) {
            int to = j.second, len = j.first;
            if (d[v] + len < d[to]) {
                q.erase(std::make_pair(d[to], to));
                d[to] = d[v] + len;
                q.insert(std::make_pair(d[to], to));
            }
        }
    }
    //std::cout << d[n - 1] << std::endl;
}

void floydWarshall(std::vector<std::vector<int>> g, int n) {
    int ans;
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (g[i][k] != -1 && g[k][j] != -1) {
                    ans = g[i][k] + g[k][j];
                    if (ans < g[i][j] || g[i][j] == -1) {
                        g[i][j] = ans;
                    }
                }
            }
        }
    }
    //std::cout << g[0][n - 1] << std::endl;
}

void bellmanFord(const std::vector<Edge> &g, int n, int m) {
    std::vector<int> d(n, LIM);
    std::vector<bool> inf(n, false);
    d[0] = 0;
    for (int j = 0; j < n - 1; ++j) {
        for (int i = 0; i < m; ++i) {
            if (d[g[i].a] != LIM) {
                if (d[g[i].b] > d[g[i].a] + g[i].cost) {
                    d[g[i].b] = d[g[i].a] + g[i].cost;
                }
            }
        }
    }
    for (int i = 0; i < m; ++i) {
        if (d[g[i].a] != LIM) {
            if (d[g[i].b] > d[g[i].a] + g[i].cost) {
                d[g[i].b] = -LIM;
            }
        }
    }
    for (int j = 0; j < n - 1; ++j) {
        for (int i = 0; i < m; ++i) {
            if (d[g[i].a] != LIM) {
                if (d[g[i].b] > d[g[i].a] + g[i].cost) {
                    d[g[i].b] = -LIM;
                }
            }
        }
    }
    //std::cout << d[n - 1] << std::endl;
}

void optimizedBellmanFord(const std::vector<Edge> &g, int n) {
    std::vector<int> d(n, LIM);
    d[0] = 0;
    bool hasChanges;
    for (int i = 0; i < n; ++i) {
        hasChanges = false;
        for (const auto& edge : g) {
            int u = edge.a;
            int v = edge.b;
            int w = edge.cost;
            if (d[u] != LIM && d[u] + w < d[v]) {
                d[v] = d[u] + w;
                hasChanges = true;
            }
        }
        if (!hasChanges) {
            break;
        }
    }
    //std::cout << d[n - 1] << std::endl;
}

int main() {

    int n, m;

    /*std::vector<int64_t> vertex_dijkstra;
    std::vector<int64_t> vertex_floydwarshall;
    std::vector<int64_t> vertex_bellmanford;
    std::vector<int64_t> vertex_obellmanford;

    std::string s = "sparse/";
    std::vector<std::vector<std::pair<int, int>>> g;
    std::vector<std::vector<int>> g1;
    std::vector<Edge> g2;

    for (int i = 10; i <= 1010; i += 50) {
        auto start = std::chrono::steady_clock::now();
        auto end = std::chrono::steady_clock::now();
        s += std::to_string(i) + ".txt";
        std::ifstream in(s);
        in >> m;
        n = i;
        g.resize(n);
        g1.resize(n);
        g2.resize(m);
        for (int k = 0; k < n; ++k) {
            g1[k].resize(n);
            for (int j = 0; j < n; ++j) {
                g1[k][j] = -1;
            }
            g1[k][k] = 0;
        }
        for (int j = 0; j < m; ++j) {
            int u, v, w;
            in >> u >> v >> w;
            u--;
            v--;
            g[u].push_back(std::make_pair(w, v));
            g1[u][v] = w;
            g2[j].a = u;
            g2[j].b = v;
            g2[j].cost = w;
        }

        start = std::chrono::steady_clock::now();
        dijkstra(g, n);
        end = std::chrono::steady_clock::now();
        vertex_dijkstra.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());

        start = std::chrono::steady_clock::now();
        floydWarshall(g1, n);
        end = std::chrono::steady_clock::now();
        vertex_floydwarshall.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());

        start = std::chrono::steady_clock::now();
        bellmanFord(g2, n, m);
        end = std::chrono::steady_clock::now();
        vertex_bellmanford.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());

        start = std::chrono::steady_clock::now();
        optimizedBellmanFord(g2, n);
        end = std::chrono::steady_clock::now();
        vertex_obellmanford.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());

        s = "sparse/";
        g.clear();
        g1.clear();
        g2.clear();
        std::cout << i << std::endl;
    }
    std::ofstream out("sparse.csv");
    for (auto i : vertex_dijkstra) {
        out << i << ',';
    }
    out << '\n';
    for (auto i : vertex_floydwarshall) {
        out << i << ',';
    }
    out << '\n';
    for (auto i : vertex_bellmanford) {
        out << i << ',';
    }
    out << '\n';
    for (auto i : vertex_obellmanford) {
        out << i << ',';
    }
    out.close();*/

    std::vector<std::vector<std::pair<int, int>>> g;
    std::vector<std::vector<int>> g1;
    std::vector<Edge> g2;

    std::map<int, int64_t> edges;

    std::string ss[3] = {"complete/", "not_complete/", "sparse/"};

    for (auto s : ss) {
        std::string old = s;
        for (int i = 10; i <= 1010; i += 50) {
            auto start = std::chrono::steady_clock::now();
            auto end = std::chrono::steady_clock::now();
            s += std::to_string(i) + ".txt";
            std::ifstream in(s);
            in >> m;
            n = i;
            g.resize(n);
            g1.resize(n);
            g2.resize(m);
            for (int k = 0; k < n; ++k) {
                g1[k].resize(n);
                for (int j = 0; j < n; ++j) {
                    g1[k][j] = -1;
                }
                g1[k][k] = 0;
            }
            for (int j = 0; j < m; ++j) {
                int u, v, w;
                in >> u >> v >> w;
                u--;
                v--;
                g[u].push_back(std::make_pair(w, v));
                g1[u][v] = w;
                g2[j].a = u;
                g2[j].b = v;
                g2[j].cost = w;
            }

            start = std::chrono::steady_clock::now();
            dijkstra(g, n);
            end = std::chrono::steady_clock::now();
            edges.insert(
                    std::make_pair(m, std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 4));

            start = std::chrono::steady_clock::now();
            floydWarshall(g1, n);
            end = std::chrono::steady_clock::now();
            edges[m] += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 4;

            start = std::chrono::steady_clock::now();
            bellmanFord(g2, n, m);
            end = std::chrono::steady_clock::now();
            edges[m] += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 4;

            start = std::chrono::steady_clock::now();
            optimizedBellmanFord(g2, n);
            end = std::chrono::steady_clock::now();
            edges[m] += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 4;

            s = old;
            g.clear();
            g1.clear();
            g2.clear();
            std::cout << i << std::endl;
        }
    }
    std::ofstream out("edges.csv");
    for (auto edge:edges) {
        out << edge.first << ',';
    }
    out << '\n';
    for (auto edge:edges) {
        out << edge.second << ',';
    }
    out.close();
    return 0;
}
