# -*- coding:utf-8 -*-

from fysom import Fysom

fsm = Fysom({"initial":"green",
             "events":[
                 {"name":"warn", "src":"green", "dst":"yellow"},
                 {"name":"panic", "src":"yellow", "dst":"red"},
                 {"name":"calm", "src":"red", "dst":"yellow"},
                 {"name":"clear", "src":"yellow", "dst":"green"}]})

class State(Fysom):
    def __init__(self):
        self.events = [
                 {"name":"warn", "src":"green", "dst":"yellow"},
                 {"name":"panic", "src":"yellow", "dst":"red"},
                 {"name":"calm", "src":"red", "dst":"yellow"},
                 {"name":"clear", "src":"yellow", "dst":"green"},]
        super(State, self).__init__(cfg={}, initial = {'state': 'green', 'event': 'init', 'defer': True}, events = self.events, callbacks=None, final=None)

    def onwarn(self, e):
        print self.current
        pass

    def onpanic(self, e):
        print self.current
        pass

    def oncalm(self, e):
        print self.current
        pass

    def onclear(self, e):
        print self.current
        pass

class ChangeState():
    def __init__(self):
        self.state = State()
        self.state.init()

    def green2Yellow(self):
        if self.state.current == "green":
            #TODO. dosomething then change state
            self.state.trigger("warn")

    def yellow2Red(self):
        if self.state.current == "yellow":
            #TODO. dosomething then change state
            self.state.trigger("panic")

    def red2Yellow(self):
        if self.state.current == "red":
            #TODO. dosomething then change state
            self.state.trigger("calm")

    def yellow2Green(self):
        if self.state.current == "yellow":
            #TODO. dosomething then change state
            self.state.trigger("clear")

if __name__ == "__main__":
    print fsm.current
    #fsm.startup()
    fsm.warn()
    print fsm.current
    fsm.panic()
    print fsm.current
    fsm.calm()
    print fsm.current
    fsm.clear()
    print fsm.current
    print "/**********************/"
    sm = State()
    sm.init()
    sm.onwarn("warn")
    sm.onpanic("panic")
    sm.oncalm("calm")
    sm.onclear("clear")
    print "/**********************/"
    chState = ChangeState()
    chState.green2Yellow()
    chState.yellow2Red()
    chState.red2Yellow()
    chState.yellow2Green()
