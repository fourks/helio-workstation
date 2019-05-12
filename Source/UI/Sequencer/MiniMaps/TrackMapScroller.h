/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

class HybridRoll;
class TrackMap;
class TrackMapScrollerScreen;
class Playhead;
class Transport;

#include "HelperRectangle.h"
#include "HybridRollListener.h"

class TrackMapScroller final :
    public Component,
    public HybridRollListener,
    private AsyncUpdater,
    private Timer
{
public:

    TrackMapScroller(Transport &transport, HybridRoll *roll);
    ~TrackMapScroller() override;

    using PageNumber = int;
    PageNumber addPage();

    void addOwnedMap(PageNumber page, Component *newTrackMap, bool shouldBringToFront);
    void switchToRoll(HybridRoll *targetRoll);
    void switchToPage(PageNumber page);

    template<typename T>
    T *findOwnedMapOfType()
    {
        for (int i = 0; i < this->trackMaps.size(); ++i)
        {
            if (T *target = dynamic_cast<T *>(this->trackMaps.getUnchecked(i)))
            {
                return target;
            }
        }
        
        return nullptr;
    }

    //===------------------------------------------------------------------===//
    // TrackScroller
    //===------------------------------------------------------------------===//
    
    void xyMoveByUser();
    void xMoveByUser();
    void resizeByUser();
    void toggleStretchingMapAlaSublime();
    
    //===------------------------------------------------------------------===//
    // Component
    //===------------------------------------------------------------------===//
    
    void resized() override;
    void paint(Graphics &g) override;
    void mouseDrag(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;
    void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
    
    //===------------------------------------------------------------------===//
    // HybridRollListener
    //===------------------------------------------------------------------===//
    
    void onMidiRollMoved(HybridRoll *targetRoll) override;
    void onMidiRollResized(HybridRoll *targetRoll) override;
    
    //===------------------------------------------------------------------===//
    // Additional horizontal dragger
    //===------------------------------------------------------------------===//
    
    class HorizontalDragHelper final : public HelperRectangle
    {
    public:
        
        explicit HorizontalDragHelper(TrackMapScroller &scrollerRef);
        void mouseDown(const MouseEvent &e) override;
        void mouseDrag(const MouseEvent &e) override;
        void paint(Graphics &g) override;
        
        class MoveConstrainer final : public ComponentBoundsConstrainer
        {
        public:
            explicit MoveConstrainer(TrackMapScroller &scrollerRef) : scroller(scrollerRef) {}
            void applyBoundsToComponent(Component &component, Rectangle<int> bounds) override;
        private:
            TrackMapScroller &scroller;
        };
        
    private:
        
        Colour colour;
        TrackMapScroller &scroller;
        ComponentDragger dragger;
        UniquePointer<ComponentBoundsConstrainer> moveConstrainer;
    };
    
protected:

    friend class HorizontalDragHelper;
    void horizontalDragByUser(Component *component, const Rectangle<int> &bounds);

    void handleAsyncUpdate() override;
    void timerCallback() override;

    void disconnectPlayhead();
    Rectangle<float> getScrollerBounds() const noexcept;
    Rectangle<int> getMapBounds(int pageOffsetY) const noexcept;
    int getPageOffsetY(PageNumber page) const noexcept;

private:
    
    Transport &transport;
    HybridRoll *roll;
    
    Rectangle<float> oldAreaBounds;
    Rectangle<float> oldMapBounds;

    UniquePointer<TrackMapScrollerScreen> screenRange;
    UniquePointer<Playhead> playhead;
    
    PageNumber currentPage = 0;
    using TrackMapPage = OwnedArray<Component>;
    Array<TrackMapPage> trackMaps;
    
    ComponentDragger helperDragger;
    UniquePointer<HelperRectangle> helperRectangle;
    
    bool mapShouldGetStretched = true;
    
};
