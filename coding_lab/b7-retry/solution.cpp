#include <iostream>

bool shouldRetry(int attempt, bool transient, int maxAttempts) {
    return attempt>=1 && maxAttempts>=1 && transient && attempt<maxAttempts;
}
