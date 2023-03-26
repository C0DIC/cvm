#include "catastring.h"
#include <stdio.h>

CataStr castr_from_cstr(char *cstr) {
    assert (cstr != NULL && "ERROR: can't make string from NULL");

    size_t len = strlen(cstr);
    char* data = (char*)malloc(len + 1);

    memcpy(data, cstr, len);
    data[len] = 0;

    return (CataStr) {
        .data = data,
        .length = len,
    };
}

CataStr castr_ltrim(char cf, CataStr cs) {
    size_t pos = 0;
    while (pos < cs.length && (cs.data[pos] == cf)) {
        pos++;
    }

    return (CataStr) {
        .data = cs.data + pos,
        .length = cs.length - pos,
    };
}

CataStr castr_rtrim(char cf, CataStr cs) {
    size_t pos = 0;
    while (pos < cs.length && (cs.data[cs.length - pos - 1] == cf)) {
        pos++;
    }

    char *new_data = (char*)malloc(cs.length + 1);
    memcpy(new_data, cs.data, cs.length - pos);

    return (CataStr) {
        .data = new_data,
        .length = cs.length - pos,
    };
}

CataStr castr_trim(char cf, CataStr cs) {    
    return castr_rtrim(cf, castr_ltrim(cf, cs));
}

bool castr_same(CataStr cs_1, CataStr cs_2) {
    if (CS_LEN(cs_1) != CS_LEN(cs_2)) return false;

    if (memcmp(cs_1.data, cs_2.data, CS_LEN(cs_1)))
        return false; else return true;
}

bool castr_has(char cf, CataStr cs) {
    if (strrchr(cs.data, cf) != NULL)
        return true;
    else return false;
}

int castr_index_of(char cf, CataStr cs) {
    size_t pos = 0;
    if (castr_has(cf, cs)) {
        while (pos < CS_LEN(cs) && cs.data[pos] != cf) pos++;
    } else return -1;
    
    return pos;
}

CataStr castr_untilc(char cf, CataStr cs) {
    int pos = castr_index_of(cf, cs);

    if (pos == -1) {
        return cs;
    } else {
        char *new_data = (char*)malloc(pos + 1);

        for (size_t i = 0; i < pos; i++) {
            new_data[i] = cs.data[i];
        }

        return (CataStr) {
            .data = new_data,
            .length = pos
        };
    }
}

CataStr castr_cutc(char cf, CataStr cs) {
    int pos = castr_index_of(cf, cs);

    if (pos == -1) {
        return cs;
    } else {
        return (CataStr) {
            .data = cs.data + pos + 1,
            .length = cs.length - pos - 1
        };
    }
}

CataStr castr_cut_by(size_t num, CataStr cs) {
    assert(num <= cs.length && "ERROR: number must be less than length (or equal)");

    return (CataStr) {
        .data = cs.data + num,
        .length = cs.length - num
    };
}

CataStr castr_walpha(CataStr cs) {
    size_t pos = 0;

    if (isalpha(cs.data[pos])) {
        while (isalpha(cs.data[pos])) {
            pos += 1;
        }
    }

    if (pos != 0) {
        char *new_data = (char*)malloc(pos + 1);
        memcpy(new_data, cs.data, pos);

        return (CataStr) {
            .data = new_data,
            .length = pos
        };
    } else {
        return (CataStr) {
            .data = "",
            .length = 0
        };
    }
}

CataStr castr_wdigit(CataStr cs) {
    size_t pos = 0;

    if (isdigit(cs.data[pos])) {
        while (isdigit(cs.data[pos]) || cs.data[pos] == '.') {
            pos += 1;
        }
    }

    if (pos != 0) {
        char *new_data = (char*)malloc(pos + 1);
        memcpy(new_data, cs.data, pos);

        return (CataStr) {
            .data = new_data,
            .length = pos
        };
    } else {
        return (CataStr) {
            .data = "",
            .length = 0
        };
    }
}

CataStr castr_cat(CataStr cs_1, CataStr cs_2) {
    char *new_data = (char*)malloc(cs_1.length + cs_2.length + 1);
    strcat(new_data, cs_1.data);
    strcat(new_data, cs_2.data);

    return CS(new_data);
}

bool castr_startswith(char *prefix, CataStr cs) {
    size_t pos = 0;
    size_t prefix_length = strlen(prefix);

    if (prefix_length != 0 && prefix_length <= cs.length) {
        while (pos < prefix_length) {
            if (cs.data[pos] != prefix[pos])
                return false;
            pos++;
        }

        if (pos == cs.length) return true;

        return true;
    }

    return false;
}

bool castr_endswith(char *suffix, CataStr cs) {
    size_t suffix_length = strlen(suffix);

    if (suffix_length <= cs.length) {
        CataStr cs_suffix = CS(cs.data + cs.length - suffix_length);
        return castr_same(cs_suffix, CS(suffix));
    }

    return false;
}

long long castr_to_lld(CataStr cs) {
    long long cs_int = 0;
    size_t pos = 0;

    if (cs.data[0] == '-') {
        pos += 1;
        while (pos < cs.length && isdigit(cs.data[pos])) {
            cs_int = cs_int*10 + (int)cs.data[pos] - '0';
            pos++;
        }

        return cs_int*-1;
    } else {
        while (pos < cs.length && isdigit(cs.data[pos])) {
            cs_int = cs_int*10 + (int)cs.data[pos] - '0';
            pos++;
        }

        return cs_int;
    }

    return 0;
}