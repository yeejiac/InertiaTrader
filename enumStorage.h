enum OrderType
{
    ROD = 1,
    IOC = 2,
    FOK = 3,
    NONE = 0
};

enum Market
{
    AfterHour = 2,
    TradeHour = 1,
    NONE = 0
};

enum Side
{
    BUY = 1,
    SELL = 2,
    NONE = 0
};

enum OrderStatus
{
    VERIFIED = 1,
    FAILED = 2,
    NONE = 0
};

enum ReportType
{
    OrderReport = 1, 
    ExecutionReport = 2, 
    ErrorReport = 3,
    NONE = 0
};
