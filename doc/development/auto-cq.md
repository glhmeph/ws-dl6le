# Auto CQ (normal FT8/FT4 operation)

Select **Auto CQ**, select **Tx6/CQ**, and use **Enable Tx** to start.
Auto CQ enables Auto Seq and CQ: First. It automatically works a caller using
existing sequencing, then returns to CQ after the final RR73 or 73 has ended.
The Auto CQ preference is saved; restoring it does not enable transmission.
Other modes and special activities keep their existing completion behavior.

Enable Tx remains checked while the final QSO is being logged, but the
scheduler blocks another transmission. With a manual log dialog, CQ resumes
after accepting or cancelling the dialog. With automatic logging, CQ resumes
after logging and transmission end. Without logging, it resumes after
transmission end. The configured TX parity and existing CQ text are retained.
The next transmission starts no earlier than the following period boundary.

Stop Tx, disabling Enable Tx, disabling Auto CQ during completion, watchdog,
rig failure, or changing the pending QSO's mode/frequency/partner cancels the
queued restart. A deliberate new station selection supersedes the restart.
Clear DX after logging is deferred until the final transmission has ended.
Auto CQ takes precedence over Disable Tx after sending 73 for supported QSOs.

## Verification without transmitting RF

Run `test_auto_cq_state` (also registered with CTest). It covers completion
before actual TX end, PTT/logging gates, duplicate events, cancellation, and
FT8/FT4 period boundaries. It does not exercise radio hardware or decode input.

Use a disconnected radio / test audio output for the following GUI checks:

1. Auto CQ off: verify original post-QSO TX shutdown with Auto Seq on.
2. Auto CQ on: CQ -> incoming report -> R-report -> own RR73 -> next CQ.
3. Incoming RR73: verify own Tx5/73 finishes before returning to Tx6.
4. Own RRR -> incoming 73: verify logging and return to CQ.
5. Repeat with Disable Tx after sending 73 both checked and unchecked.
6. Repeat with logging off, automatic logging, and a manual log dialog.
   Hold the dialog open for multiple periods: no additional TX is allowed.
7. Stop Tx while the final message or log dialog is pending. Closing the
   dialog afterward must not restart CQ. Also check UDP halt and watchdog.
8. After cancelling completion, select a new station before accepting the
   old log dialog: its acceptance must not clear the new station's fields.
9. Check both TX parities, directed CQ text, Clear DX call/grid settings,
   FT4 and FT8, and consecutive QSOs with different partners.
10. Change mode/special activity and restart the application: no spontaneous
    TX; Auto CQ must not change unsupported modes' QSO completion behavior.
