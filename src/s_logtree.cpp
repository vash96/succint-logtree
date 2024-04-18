#include <bits/stdc++.h>
#include <bit>
using namespace std;


// TODO: multi-tree via base32 expression
class s_log32tree {
public:
    uint32_t nbits_, nblks_, h_, fblk_;
    vector<uint32_t> v_;
public:
    s_log32tree(uint32_t n) {
        h_ = 0;
        nblks_ = 1;
        uint32_t sz = 1;
        while(32*nblks_ < n) {
            h_ += 1;
            nblks_ *= 32;
            sz += nblks_;
        }
        nbits_ = 32*nblks_;
        fblk_ = (nblks_ - 1) / 31;
        v_.resize(sz);
    }

    bool Access(uint32_t i) const {
        return (v_[fblk_ + i / 32] & ((1u << (31 - (i % 32))))) > 0;
    }

    uint32_t Leftmost1() const {
        uint32_t i = 0;
        for(auto l=0; l<h_; ++l) {
            i = 32*i + 1 + countl_zero(v_[i]);
        }
        return i < v_.size() ? 32*(i - fblk_) + countl_zero(v_[i]) : -1;
    }

    void Set(uint32_t i, bool val) {
        const uint32_t x = val << (31 - (i % 32));
        uint32_t blk = fblk_ + i / 32;
        v_[blk] = val ? (v_[blk] | x) : (v_[blk] & ~x);

        for(auto l=0; l<h_; ++l) {
            const uint32_t pblk = blk / 32;
            const uint32_t k = blk - 32 * pblk - 1;
            val = v_[blk] > 0;
            v_[pblk] = v_[pblk] | (val << (31 - k));
            blk = pblk;
        }
    }
};

int main()
{
    int nbits = 33*32;
    s_log32tree tree(nbits);

    cout << "I want to store " << nbits << " bits\n";
    cout << "nblks_ = " << tree.nblks_ << "\nh_ = " << tree.h_ << "\nfblk_ = " << tree.fblk_ << "\n";
    cout << "v_.size() = " << tree.v_.size() << "\n";
    cout << "bitsize = " << tree.nbits_ << "\n";
    cout << "overhead = " << (float(tree.nbits_ - nbits) / nbits) << " times more than " << nbits << "\n";

    tree.Set(1, 1);
    tree.Set(13, 1);
    tree.Set(30, 1);

    for(int i=0; i<tree.nbits_; ++i)
        cout << tree.Access(i);
    cout << endl;


    cout << "Leftmost 1 in position " <<  tree.Leftmost1() << "\n";

    cout << endl;
}