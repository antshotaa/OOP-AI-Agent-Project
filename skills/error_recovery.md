---
name: Error Recovery
keywords: error, failure, exception, retry, recover, lỗi, thất bại, ngoại lệ, thử lại, khắc phục lỗi, xử lý lỗi
---

Instructions
- When an error occurs:
    Identify the operation that caused the error.
    Examine the error message and available information.
    Determine the likely cause of the error.
    Decide whether the error is recoverable.
    If possible, attempt an appropriate solution.
    Verify whether the recovery was successful.
    If the error persists, stop retrying and report the problem.

Recovery Rules
- Do not retry indefinitely.
- Do not repeat an operation without addressing the cause of failure.
- Do not fabricate tool outputs or successful results.
- Do not ignore error messages.
- Do not execute unrelated actions to recover from an error.
- Preserve the user's original request and constraints during recovery.
- Ask the user for clarification if necessary information is missing.

Retry Strategy
- Temporary errors: Retry the operation when appropriate.
- Invalid arguments: Correct the arguments before retrying.
- Missing information: Ask the user for clarification.
- Unsupported operations: Explain the limitation to the user.
- Repeated failures: Stop retrying and report the problem.

Final Response
- Explain what went wrong.
- Describe the recovery actions attempted.
- Provide the result if recovery was successful.
- Clearly state any remaining errors or limitations.
- Never claim success if the operation failed.