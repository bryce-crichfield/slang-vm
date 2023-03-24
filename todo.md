Floating Point Arithmetic
    - Implement Subroutines
Bitwise Arithmetic
    - Define enums and decode logic
    - Implement subroutines
Branch Instructions
    - Define enums and decode logic
    - Implement subroutines
Memory Instructions
    - Implement subroutines
    - Implement block memory allocation
    - Define API interface for memory management


LoadI   0x01
Alloc   0x01
Dup
Rot
Swap
StoreM  0x00
LoadM   0x00
Halt

Drop    :: [A] -> []
Dup     :: [A] -> [A, A]
Swp     :: [A, B] -> [B, A]
Rot     :: [A, B, C] -> [C, A, B]

StoreR  :: [Register, Value] -> []
LoadR   :: [Register] -> [Value]

StoreM  :: [Address, Value] -> []
LoadM   :: [Address] -> [Value]