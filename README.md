# Always Glowing Master Sword

Android ARM64 Dusklight native-mod prototype.

## Build
Push this repository to GitHub and open **Actions → Build Android Dusk → Run workflow**.
If compilation succeeds, download the `always-glowing-master-sword-android` artifact.

The workflow uses Android NDK 29, arm64-v8a and Android API 28, matching the
current official Dusklight mod-template Android job.

## Safety note
The source deliberately uses the named `mNoResetFlg0` field rather than an
unverified raw object offset. If the pinned Dusklight headers don't expose that
field, compilation should fail instead of producing a potentially unsafe patch.
