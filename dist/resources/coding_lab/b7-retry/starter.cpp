#include <iostream>

bool shouldRetry(int attempt, bool transient, int maxAttempts) {
    // TODO: combine eligibility and the attempt limit.
    return transient;
}
