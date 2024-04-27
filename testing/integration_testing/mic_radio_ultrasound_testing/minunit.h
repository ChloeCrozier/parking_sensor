// Tiny unit testing framework -- courtesy of MinUnit (jera.com)
// I am using the same minunit.h from the custommodule we used in class

#ifndef MIN_UNIT_H
#define MIN_UNIT_H

#define mu_assert(message, test) do { assertions++; if (!(test)) { \
                                  Serial.println("\tERROR\t" message); \
                                  return message; } } while (0)
                                  
#define mu_run_test(test) do { char *message = test(); tests_run++; Serial.print(tests_run); Serial.print(": ");\
                                if (message) { \
                                  Serial.print("FAILED: "); \
                                  Serial.println(#test); \
                                  Serial.print("\t"); \
                                  Serial.println(message);\
                                  return message; \
                                 } else { \
                                  tests_passed++; \
                                  Serial.print("PASSED: "); \
                                  Serial.println(#test); \
                                 } \
                              } while (0)


#define mu_assert_equal(message, expected, given) do { if (!(expected == given)) { assertions++; \
                                                          Serial.print("\tERROR\t" message " ("); \
                                                          Serial.print(expected); \
                                                          Serial.print(" != "); \
                                                          Serial.print(given); \
                                                          Serial.println(")"); \
                                                          return message; } } while (0)
                                
extern int tests_run;
extern int tests_passed;
extern int assertions;

#endif
