#include <iostream>

class Toy {
private:
    std::string name;
public:
    Toy (std::string inName) : name(inName) {}
    Toy () : name("someToy") {}
    void setName (std::string inName) {
        name = inName;
    }
};

template<typename T>
class Shared_ptr_toy {
  private:
    T* pointer = nullptr;
    int *count = nullptr;
  public:
    Shared_ptr_toy(){};
    Shared_ptr_toy (T *other) {
        pointer = other;
        count = new int(1);
    }
    Shared_ptr_toy(std::string inName) {
        pointer = new Toy(inName);
        count = new int(1);
    }
    Shared_ptr_toy (const Shared_ptr_toy &other) {
        pointer = other.pointer;
        count = other.count;
        ++*count;
    }
    Shared_ptr_toy& operator= (const Shared_ptr_toy &other) {
        if (pointer == other.pointer) {
            return *this;
        }
        if (pointer != nullptr) {
            delete pointer;
            delete count;
        }
        count = other.count;
        ++*count;
        pointer = other.pointer;
        return *this;
    }
    ~Shared_ptr_toy() {
        if (*count == 1) {
            delete count;
            delete pointer;
        } else {
            --*count;
        }
    }
    friend Shared_ptr_toy<T> make_shared_toy(std::string inName);
};

template<typename T>
Shared_ptr_toy<T> make_shared_toy (std::string inName) {
    Shared_ptr_toy<T> tmpObj = Shared_ptr_toy<T>(new T(inName));
    return tmpObj;
}

template<typename T>
Shared_ptr_toy<T> make_shared_toy (Shared_ptr_toy<T> &other) {
    Shared_ptr_toy<T> tmpObj = other;
    return tmpObj;
}

int main() {
    {
        Shared_ptr_toy<Toy> toy1;
        Shared_ptr_toy<Toy> toy2("ball");
        toy1 = toy2;
        Shared_ptr_toy<Toy> toy3(new Toy("bone"));
        toy3 = toy2;
        Shared_ptr_toy<Toy> toy4(toy3);
    }

    Shared_ptr_toy<Toy> toy1 = make_shared_toy<Toy>("bone");
    Shared_ptr_toy<Toy> toy2 = make_shared_toy<Toy>(toy1);
    Shared_ptr_toy<Toy> toy3;
    toy3 = toy1;
    Shared_ptr_toy<Toy> toy4(toy3);

    return 0;
}
