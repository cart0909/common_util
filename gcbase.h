#ifndef GCBASE_H
#define GCBASE_H

class A {
public:
    A() {
        count = new int(1);
    }

    A(const A& rhs) {
        if(count != nullptr) {
            *count -= 1;
            if(*count == 0) {
                cout << "delete object" << endl;
                delete count;
            }
        }
        count = rhs.count;
        *count += 1;
    }

    ~A() {
        *count -= 1;
        if(*count == 0) {
            cout << "delete object" << endl;
            delete count;
        }
    }

    A& operator=(const A& rhs) {
        if(count != nullptr) {
            *count -= 1;
            if(*count == 0) {
                cout << "delete object" << endl;
                delete count;
            }
        }
        count = rhs.count;
        *count += 1;
    }

    int GetRefCount() const {
        return *count;
    }

private:
    int *count = nullptr;
};

#endif // GCBASE_H
