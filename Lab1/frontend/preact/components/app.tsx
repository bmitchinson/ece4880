import { h, Component } from 'preact';
import { Router } from 'preact-router';

import Header from './header/header';

interface MyProps {}
interface MyState {}

export default class App extends Component<MyProps, MyState> {
    /** Gets fired when the route changes.
     *	@param {Object} event		"change" event from [preact-router](http://git.io/preact-router)
     *	@param {string} event.url	The newly routed URL
     */
    currentUrl;

    handleRoute = e => {
        this.currentUrl = e.url;
    };

    render() {
        return (
            <div id="app">
                <Header />
            </div>
        );
    }
}
