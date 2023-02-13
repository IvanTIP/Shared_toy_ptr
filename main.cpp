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

struct ControlBlock {
    Toy obj;
    int count;
};

template<typename T>
class Shared_ptr_toy {
  private:
    T* pointer = nullptr;
    int *count = nullptr;
    ControlBlock* cbPtr = nullptr;
  public:
    Shared_ptr_toy(ControlBlock *cbOther, std::string &inName) {
        cbPtr = cbOther;
        cbPtr->obj.setName(inName);
        cbPtr->count = 1;
    }
    Shared_ptr_toy() {}
    Shared_ptr_toy (T *other) {
        pointer = other;
        count = new int(1);
    }
    Shared_ptr_toy (const Shared_ptr_toy &other) {
        if (other.cbPtr != nullptr) {
            ++other.cbPtr->count;
            cbPtr = other.cbPtr;
        } else {
            pointer = other.pointer;
            count = other.count;
            ++*count;
        }
    }
    Shared_ptr_toy& operator= (const Shared_ptr_toy &other) {
        if (other.cbPtr != nullptr) {
            if (cbPtr == other.cbPtr) {
                return *this;
            }
            if (cbPtr != nullptr) {
                delete cbPtr;
            }
            ++other.cbPtr->count;
            cbPtr = other.cbPtr;
        } else {
            if (this->pointer == other.pointer) {
                return *this;
            }
            count = other.count;
            ++*count;
            if (pointer != nullptr) {
                delete pointer;
            }
            pointer = other.pointer;
            return *this;
        }
    }
    ~Shared_ptr_toy() {
        if (cbPtr != nullptr) {
            if (cbPtr->count != 1) {
                --cbPtr->count;
            } else {
                delete cbPtr;
            }
        }else {
            if (*count == 1) {
                delete count;
                delete pointer;
            } else {
                --*count;
            }
        }
    }
    friend Shared_ptr_toy<T> make_shared_toy(std::string inName);
};

template<typename T>
Shared_ptr_toy<T> make_shared_toy (std::string inName) {
    Shared_ptr_toy<T> tmpObj = Shared_ptr_toy<T>(new ControlBlock, inName);
    return tmpObj;
}

template<typename T>
Shared_ptr_toy<T> make_shared_toy (Shared_ptr_toy<T> &other) {
    Shared_ptr_toy<T> tmpObj = other;
    return tmpObj;
}

int main() {
    {
        Toy *toy = new Toy("ball");
        Shared_ptr_toy<Toy> toy1(toy);
        Shared_ptr_toy<Toy> toy2 = toy1;
        Shared_ptr_toy<Toy> toy3;
        toy3 = toy2;
    }

    Shared_ptr_toy<Toy> toy1 = make_shared_toy<Toy>("bone");
    Shared_ptr_toy<Toy> toy2 = make_shared_toy<Toy>(toy1);
    Shared_ptr_toy<Toy> toy3;
    toy3 = toy1;

    return 0;
}
