#include "mods/svc/hook.hpp"
#include "mods/service.hpp"

// Game include. daAlink_c is required for the member-function ABI.
#include "d/actor/d_a_alink.h"

DEFINE_MOD();
IMPORT_SERVICE(HookService, svc_hook);

// Private/non-public member: resolve by symbol name.
// For member functions the first ABI argument is `this`.
DEFINE_HOOK_SYMBOL("daAlink_c::checkLightSwordMtrl",
                   void(daAlink_c*),
                   CheckLightSwordMtrl);

static void on_light_sword_post(ModContext*, void* args, void*, void*) {
    auto* link = mods::arg<daAlink_c*>(args, 0);
    if (link == nullptr) {
        return;
    }

    // Prototype based on the Lazy Tweaks behavior identified during comparison:
    // vanilla checkLightSwordMtrl() owns bit 0x00100000 of mNoResetFlg0.
    // Restore it after vanilla performs its normal check.
    //
    // IMPORTANT: the build is intentionally allowed to fail here if the pinned
    // Dusklight game headers do not expose this field. That is safer than
    // hard-coding an unverified object offset.
    link->mNoResetFlg0 |= 0x00100000u;
}

extern "C" {

MOD_EXPORT ModResult mod_initialize(ModError* error) {
    ModResult result =
        mods::hook::add_post<CheckLightSwordMtrl>(on_light_sword_post);
    if (result != MOD_OK) {
        return mods::set_error(
            error, result,
            "failed to hook daAlink_c::checkLightSwordMtrl");
    }
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    // Dusklight removes registered hooks when the mod is detached.
    return MOD_OK;
}

}
