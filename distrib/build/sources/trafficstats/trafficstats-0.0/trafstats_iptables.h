
#define MAX_RULES 200

// things we like to know about a rule,
// fetch_counts returns an array of these
struct iptable_data{
    u_int64_t pcnt,bcnt; // byte,packet count
    char iniface[IFNAMSIZ]; // incomming interface
    char outiface[IFNAMSIZ]; // outgoing interface
    struct in_addr src,dst,smsk,dmsk; // source, destination addresses and masks
    u_int16_t proto; // protocol this rule is for 0 = any
};
    
    
int fetch_counts(const char *, struct iptable_data *, int);
