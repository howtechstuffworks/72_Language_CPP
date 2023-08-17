#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

// ----------------------------------------------------------------------------
class UniqueFd {
public:
  UniqueFd(int fdesc, size_t fsize) : fdesc_(fdesc), fsize_(fsize) {}
  ~UniqueFd() {
  }

  int get_fdesc() const { return fdesc_; }
private:
  int fdesc_;
  size_t fsize_;
};

std::unordered_map<std::string, std::shared_ptr<UniqueFd>> ufdmap;

// ----------------------------------------------------------------------------

std::vector<std::string> libs = {
  "lib1", "lib1"
  //"lib2", "lib3", "lib2", "lib1", "lib3", "lib2", "lib1", "lib1",
};

void GetUniqueFileHandle(std::string lib, std::shared_ptr<UniqueFd>& ufd) {
  static int curr_fd = 1;
  size_t curr_size = 4096;
  if (ufdmap.cend() == ufdmap.find(lib)) {
    ufdmap.insert(std::make_pair(lib, std::make_shared<UniqueFd>(curr_fd++, curr_size)));
  }
  ufd = ufdmap[lib];
}

class FBI {
public:
  FBI(std::string lib, std::shared_ptr<UniqueFd> ufd) : lib_(lib), ufd_(ufd) {}
  ~FBI() {}
  std::string get_libname() const {return lib_; }
private:
  std::string lib_;
  std::shared_ptr<UniqueFd> ufd_;
};

std::vector<FBI*> fbi_vec;

int main() {
  std::shared_ptr<UniqueFd> ufd;
  for (auto& lib : libs) {
    GetUniqueFileHandle(lib, ufd);
    std::cout<<lib<<" : "<<ufd.use_count()<<std::endl;
    fbi_vec.push_back(new FBI(lib, ufd));
    std::cout<<lib<<" : "<<ufd.use_count()<<std::endl;
  }

  int dummy = 1;

  // Cleanup
  std::cout<<std::endl<<"At Cleanup"<<std::endl;
  for (auto& fbi: fbi_vec) {
    std::string lib_name = fbi->get_libname();
    std::cout<<lib_name<<" : "<<ufd.use_count()<<std::endl;
    delete fbi;
    std::cout<<lib_name<<" : "<<ufd.use_count()<<std::endl;
    if (ufd.use_count() == 2) {
      ufdmap.erase(lib_name);
    }
  }

  std::cout<<"Final: "<<ufd.use_count()<<std::endl;

  return 0;
}