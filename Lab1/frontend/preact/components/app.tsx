import { h, Component } from 'preact';

import Header from './header/header';
import ComponentGrid from './componentGrid/componentGrid';

interface MyProps {}
interface MyState {}

export default class App extends Component<MyProps, MyState> {
    render() {
        return (
            <div id="app">
                <Header />
                <ComponentGrid />
            </div>
        );
    }
}
