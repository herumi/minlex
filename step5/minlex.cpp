//------------------------------------------------------------------------
#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <fstream>
//------------------------------------------------------------------------
bool hlb_table[8][6] = {{true, true, true, true, true, true}, {true, false, true, false, false, false}, {false, true, false, false, true, false}, {true, true, false, false, false, false}, {false, false, false, true, false, true}, {false, false, true, true, false, false}, {false, false, false, false, true, true}, {true, true, true, true, true, true}};
int perm3[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
//------------------------------------------------------------------------
class Sudoku {
public:
  unsigned char data[81];

  Sudoku() {
    for (int i = 0; i < 81; i++) {
      data[i] = 9;
    }
  }

  Sudoku(const char* str) {
    for (int i = 0; i < 81; i++) {
      data[i] = str[i] - '0';
    }
  }

  Sudoku & operator=(Sudoku const &g2) {
    for (int i = 0; i < 81; i++) {
      data[i] = g2.data[i];
    }
    return (*this);
  }

  Sudoku(unsigned char data2[81]) {
    for (int i = 0; i < 81; i++) {
      data[i] = data2[i];
    }
  }

  bool operator==(const Sudoku &rhs) const {
    for (int i = 0; i < 81; i++) {
      if (rhs.data[i] != this->data[i])return false;
    }
    return true;
  }
  bool operator!=(const Sudoku &rhs) const {
    return !(*this == rhs);
  }

  bool operator<(const Sudoku &rhs) const {
    for (int i = 0; i < 81; i++) {
      if (rhs.data[i] != this->data[i]) {
        return (this->data[i] < rhs.data[i]);
      }
    }
    return false;
  }

  void headline_bits(int &h1, int &h2, int &h3) {
    h1 = 0;
    h2 = 0;
    h3 = 0;
    for (int i = 0; i < 3; i++) {
      if (data[i] != 0) h1 += (1 << (2 - i));
      if (data[i + 3] != 0) h2 += (1 << (2 - i));
      if (data[i + 6] != 0) h3 += (1 << (2 - i));
    }
  }

  int headline_index(void) {
    std::vector<int> a(3, 0);
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (data[j + i * 3] != 0) a[i]++;
      }
    }
    std::sort(a.begin(), a.end());
    int index = 0;
    for (int i = 0; i < 3; i++) {
      int t = ((1 << a[i]) - 1) << (2 - i) * 3;
      index += t;
    }
    return index;
  }

  int headbox_index(bool sort) {
    int min = 16 * 3 + 4 * 3 + 3;
    for (int k = 0; k < 3; k++) {
      std::vector<int> a(3, 0);
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if (data[j + i * 3 + k * 9] != 0) a[i]++;
        }
      }
      if (sort) {
        std::sort(a.begin(), a.end());
      }
      int index = 0;
      for (int i = 0; i < 3; i++) {
        int t = ((1 << a[i]) - 1) << (2 - i) * 3;
        index += t;
      }
      if (index < min) {
        min = index;
      }
    }
    return min;
  }

  void show(void) {
    for (int i = 0; i < 81; i++) {
      printf("%c", data[i] + '0');
    }
    printf("\n");
  }

  std::string str(void) {
    std::stringstream ss;
    for (int i = 0; i < 81; i++) {
      ss << (int)(data[i]);
    }
    return ss.str();
  }

  Sudoku head(void) {
    unsigned char data2[81] = {};
    for (int i = 0; i < 27; i++) {
      data2[i] = data[i];
    }
    return Sudoku(data2);
  }

  Sudoku renumbering(void) {
    unsigned char data2[81] = {};
    int h[9] = {};
    int index = 0;
    for (int i = 0; i < 81; i++) {
      if (data[i] == 0)continue;
      unsigned char c = data[i];
      if (h[c - 1] == 0) {
        index++;
        h[c - 1] = index;
      }
      data2[i] = h[c - 1];
    }
    return Sudoku(data2);
  }

  Sudoku perm_restrbox(int ai[3], int aj[3]) {
    unsigned char data2[81];
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 9; j++) {
        data2[j + i * 9] = data[j + i * 9];
        data2[j + (i + 3) * 9] = data[j + (ai[i] + 3) * 9];
        data2[j + (i + 6) * 9] = data[j + (aj[i] + 6) * 9];
      }
    }
    return Sudoku(data2);
  }

  Sudoku perm_columns(int ai[3], int aj[3], int ak[3]) {
    unsigned char data2[81];
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 3; j++) {
        data2[j + i * 9] = data[ai[j] + i * 9];
        data2[j + 3 + i * 9] = data[aj[j] + 3 + i * 9];
        data2[j + 6 + i * 9] = data[ak[j] + 6 + i * 9];
      }
    }
    return Sudoku(data2);
  }

  Sudoku perm_toprbox(int a[3]) {
    unsigned char data2[81];
    for (int i = 0; i < 9; i++) {
      data2[i] = data[i + a[0] * 9];
      data2[i + 9] = data[i + a[1] * 9];
      data2[i + 18] = data[i + a[2] * 9];
    }
    for (int i = 27; i < 81; i++) {
      data2[i] = data[i];
    }
    return Sudoku(data2);
  }

  Sudoku perm_cbox(int a[3]) {
    unsigned char data2[81];
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 3; j++) {
        data2[j + i * 9] = data[a[0] * 3 + j + i * 9];
        data2[j + 3 + i * 9] = data[a[1] * 3 + j + i * 9];
        data2[j + 6 + i * 9] = data[a[2] * 3 + j + i * 9];
      }
    }
    return Sudoku(data2);
  }
  Sudoku perm_rbox(int a[3]) {
    unsigned char data2[81];
    for (int i = 0; i < 27; i++) {
      data2[i] = data[i + a[0] * 27];
      data2[i + 27] = data[i + a[1] * 27];
      data2[i + 54] = data[i + a[2] * 27];
    }
    return Sudoku(data2);
  }
  Sudoku transpose(void) {
    unsigned char data2[81];
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        data2[j + i * 9] = data[i + j * 9];
      }
    }
    return Sudoku(data2);
  }
};
//------------------------------------------------------------------------
class MinlexSearcher {
private:
  Sudoku min, max;
public:
  void perm_restrbox(Sudoku &g) {
    for (auto ai : perm3) {
      for (auto aj : perm3) {
        Sudoku g2 = g.perm_restrbox(ai, aj).renumbering();
        if (g2 < min) {
          min = g2;
        }
      }
    }
  }

  void perm_columns(Sudoku &g) {
    int h1, h2, h3;
    g.headline_bits(h1, h2, h3);
    for (int k = 0; k < 6; k++) {
      if (!hlb_table[h3][k])continue;
      int *ak = perm3[k];
      for (int j = 0; j < 6; j++) {
        if (!hlb_table[h2][j])continue;
        int *aj = perm3[j];
        for (int i = 0; i < 6; i++) {
          if (!hlb_table[h1][i])continue;
          int *ai = perm3[i];
          Sudoku g2 = g.perm_columns(ai, aj, ak).renumbering();
          if (min.head() < g2.head()) continue;
          perm_restrbox(g2);
        }
      }
    }
  }
  void perm_toprbox(Sudoku &g, int hb_min) {
    for (auto a : perm3) {
      Sudoku g2 = g.perm_toprbox(a);
      if (g2.headline_index() > hb_min)continue;
      perm_columns(g2);
    }
  }
  void perm_cbox(Sudoku &g, int hb_min) {
    for (auto a : perm3) {
      Sudoku g2 = g.perm_cbox(a);
      if (g2.headbox_index(false) > hb_min)continue;
      perm_toprbox(g2, hb_min);
    }
  }

  std::string search(Sudoku &g) {
    min = max;
    std::vector<Sudoku> v;
    std::vector<int> vi;
    for (auto a : perm3) {
      Sudoku g2 = g.perm_rbox(a);
      v.push_back(g2);
      vi.push_back(g2.headbox_index(true));
    }
    Sudoku gt = g.transpose();
    for (auto a : perm3) {
      Sudoku g2 = gt.perm_rbox(a);
      v.push_back(g2);
      vi.push_back(g2.headbox_index(true));
    }
    int hb_min = *std::min_element(vi.begin(), vi.end());
    for (size_t i = 0; i < v.size(); i++) {
      if (vi[i] > hb_min) continue;
      perm_cbox(v[i], hb_min);
    }
    return min.str();
  }
};
//------------------------------------------------------------------------
void
show(std::vector<int> &v) {
  for (auto i : v) {
    std::cout << i;
  }
  std::cout << std::endl;
}
//------------------------------------------------------------------------
void
test(void) {
  std::string str = "207005000000340000150000009005000001040000320000016500000002084700000010010580000";
  std::string ans = "000000012000034005006007300001300007053080000080000100010005090200100000700400030";

  str = "400030100908000700000000096040100000070000009500200070107980000020005080000004600";
  ans = "000000012000003400056000700000040800001072500820000060000085009008700040700900000";

  str = "009000000500080200400020090010300060080600007050000900608730000000106500000040700";
  ans = "000000001000002030014050200000000467300048000700200000003000000106000093980075000";

  Sudoku g(str.c_str());
  MinlexSearcher s;
  g.show();
  std::string min = s.search(g);
  std::cout << min << std::endl;
  if (min == ans) {
    std::cout << "OK" << std::endl;
  } else {
    std::cout << "NG" << std::endl;
  }
}
//------------------------------------------------------------------------
void
input(const char*filename) {
  std::string line;
  MinlexSearcher s;
  std::ifstream ifs(filename);
  while (getline(ifs, line)) {
    Sudoku g(line.c_str());
    std::cout << s.search(g) << std::endl;
  }
}
//------------------------------------------------------------------------
int
main(int argc, char **argv) {
  if (argc > 1) {
    input(argv[1]);
  } else {
    test();
  }
}
//------------------------------------------------------------------------
