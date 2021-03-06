/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2016 (git@davedissian.com)
 */
#pragma once

namespace dw {

/// Class used to keep track of entities being updated
class DW_API SceneManager : public Object {
public:
    DW_OBJECT(SceneManager);

    SceneManager(Context* context);
    ~SceneManager();

    /// Calls update on each entity
    /// @param dt Time elapsed
    void update(float dt);

    /// Calls PreRender on each entity
    void preRender(Camera* camera);

private:
};
}
