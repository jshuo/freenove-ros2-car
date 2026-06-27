# 3-channel infrared line sensor state
bool left     # CH1 (GPIO14)
bool center   # CH2 (GPIO15)
bool right    # CH3 (GPIO23)
int32 encoded # 3-bit encoded: (left<<2)|(center<<1)|right
