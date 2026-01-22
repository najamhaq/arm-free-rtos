
Idle 
├─ btnDown → PressDebounce (start press debounce)
└─ btnUp   → Idle

PressDebounce 
├─ btnUp               → Idle
├─ btnDown + time>=D   → Pressed (emit Press)
└─ btnDown + time<D    → PressDebounce

Pressed
├─ btnDown → Held
└─ btnUp   → ReleaseDebounce (start release debounce)

Held 
├─ btnDown → Held
└─ btnUp   → ReleaseDebounce

ReleaseDebounce 
├─ btnDown            → PressDebounce
├─ btnUp + time>=D    → Released (emit Release)
└─ btnUp + time<D     → ReleaseDebounce

Released 
├─ btnDown → PressDebounce
└─ btnUp   → Idle
