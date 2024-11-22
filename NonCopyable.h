#ifndef NON_COPYABLE_H_
#define NON_COPYABLE_H_

/** Inherit from this to make the class non-copyable. */
class NonCopyable
{
public:
    NonCopyable() = default;
    ~NonCopyable() = default;
protected:
    NonCopyable(const NonCopyable& other) = delete;
    NonCopyable& operator=(const NonCopyable& other) = delete;
};

#endif // NON_COPYABLE_H_