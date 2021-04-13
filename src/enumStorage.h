enum class OrderType
{
    ROD = 1,
    IOC = 2,
    FOK = 3,
    NONE = 0
};

enum class Market
{
    AfterHour = 2,
    TradeHour = 1,
    NONE = 0
};

enum class Side
{
    BUY = 1,
    SELL = 2,
    NONE = 0
};

enum class OrderStatus
{
    VERIFIED = 1,
    FAILED = 2,
    NONE = 0
};

enum class ReportType
{
    OrderReport = 1, 
    ExecutionReport = 2, 
    ErrorReport = 3,
    NONE = 0
};
