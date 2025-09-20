# Attorney Pattern Helper Macros

The **Attorney Pattern Helper Macros** provide an extensible way to grant selective, compile-time-controlled access to private members (fields and methods) of C++ classes. This module enables advanced encapsulation techniques by leveraging the Attorney-Client idiom with C++11/17 features, and is especially suited for Unreal Engine development or any scenario requiring granular friend access.

## Features

- **Granular, Type-safe Friend Access** to private or protected members.
- **Macro-based Interface** for quick and error-free usage.
- **Compile-time Forwarding** for methods (supports void and non-void returns).
- **Read-only Field Exposure** via safe accessor macros.
- **Multiple Party Support** for tests, controllers, and tightly-coupled systems.
- **Encapsulation without Compromise**, using documented C++ idioms.

## Tech Stack

- **Language**: Modern C++ (11 or later, tested with 17).
- **Environment**: Engine-agnostic, designed for Unreal Engine compatibility.
- **Core Mechanism**: Macros, templates, and the Attorney-Client pattern.

## Installation

### Prerequisites

- C++11/17 compatible toolchain.
- Include `Helpers.h` in your project.
- (For Unreal) Update `.build.cs` to reference the containing module.

### Steps

1. Copy `Helpers.h` into your project's include directory.
2. Include it in the private section of any class requiring controlled access.
3. Use the macros to declare attorney classes and expose private functionality.

## Usage

Suppose you have a class with sensitive members:

```cpp
// MySecretSubsystem.h
#include "Helpers.h"

class UMySecretSubsystem : public UObject
{
    GENERATED_BODY()

private:
    FString SecretValue;
    void ResetState();
    int32 InternalComputation(int32 X) const { return X * 2; }

    BEFRIEND_ATTORNEY(MySecretAttorney)
};
```

### Exposing Access with an Attorney

You want to expose only particular private fields or methods
```cpp
// MySecretAttorney.h
#include "Helpers.h"
DECLARE_ATTORNEY_BEGIN(MySecret, UMySecretSubsystem)
    // Allow UMySecretController access to private members
    BEFRIEND_PARTY(UMySecretController)

    // Expose a private field
    EXPOSE_CLIENT_FIELD(SecretValue)

    // Expose a private const method
    EXPOSE_CLIENT_METHOD(InternalComputation)
DECLARE_ATTORNEY_END()
```

### Consuming the Attorney in Another Class

```cpp
// UMySecretController.cpp
void UMySecretController::RevealSecret(UMySecretSubsystem* Target)
{
    FString Value = FMySecretAttorney::GetSecretValue(Target);
    UE_LOG(LogTemp, Log, TEXT("Secret: %s"), *Value);

    int32 Result = FMySecretAttorney::InternalComputation(Target, 42);
    UE_LOG(LogTemp, Log, TEXT("Computation Result: %d"), Result);
}
```

### Unit Testing Internals (Advanced)

```cpp
// In automated test (accessible only to test class)
DECLARE_ATTORNEY_BEGIN(MySecretTest, UMySecretSubsystem)
    BEFRIEND_PARTY(FMyTestClass)
    EXPOSE_CLIENT_FIELD(SecretValue)
DECLARE_ATTORNEY_END()

bool FMyTestClass::RunTest()
{
    UMySecretSubsystem* TestObj = NewObject<UMySecretSubsystem>();
    // Directly access 'SecretValue' for validation
    TestEqual(TEXT("SecretValue should be empty"), 
        FMySecretTestAttorney::GetSecretValue(TestObj), TEXT(""));
    return true;
}
```

## API Documentation

| Macro                  | Description                                                                                                      |
|------------------------|------------------------------------------------------------------------------------------------------------------|
| `BEFRIEND_ATTORNEY(X)` | Establishes friendship for `FXAttorney` with the client class.                                                  |
| `DECLARE_ATTORNEY_BEGIN(Name, ClientType)` | Declares an attorney class for `ClientType`.                                             |
| `BEFRIEND_PARTY(Y)`    | Grants friend access to another class `Y` through this attorney.                                                 |
| `EXPOSE_CLIENT_FIELD(Field)` | Creates a getter for a private field.                                                                    |
| `EXPOSE_CLIENT_METHOD(MethodName)` | Forwards calls with arbitrary arguments to a private method.                                      |
| `DECLARE_ATTORNEY_END()` | Closes the attorney class declaration.                                                                        |

## Contributing

- Follow engine (or your project’s) naming conventions.
- Add tests for each new pattern and macro.
- Document usage of new macros and expose only what’s critically needed.
- Include Doxygen-style comments for new APIs and affordances.
- Use code reviews for any changes to macro logic or the attorney pattern core.

## License

See LICENSE.md for open source licensing details.
