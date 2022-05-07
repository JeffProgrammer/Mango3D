local MainMenuGui = Gui.create("MainMenuGui", {
    {
        class = "Button",
        click = "onClickPlay",
        position = { 10, 20 },
        extent = { 100, 50 }
    },
    {
        class = "Text",
        position = { 10, 40 },
        text = "16 mspf"
        name = "mspfCounter"
    }
});

function MainMenuGui:onOpenGui()
    self.interval = self:interval(1000, "updateGui")
end

function MainMenuGui:onCloseGui()
    self:cancel(self.interval)
end

function MainMenuGui:onClickPlay()
    print "Clicked!"
end

function MainMenuGui:updateGui()
    local mspf = (1000 / engine.fps) .. "mspf"
    self:setText("mspfCounter", mspf)
end