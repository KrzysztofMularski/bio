#include "manager.h"
#include "printPatterns.h"
#include "instances.h"

int main() {
    
    // srand(time(NULL));

    populateInstances();
    
    Manager manager(
        instances
    );

    manager.runAll();

    return 0;
}